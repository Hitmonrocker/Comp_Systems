//Includes
#include "Simple_SCH.h"
//Defines

//Global Vars
static ucontext_t cxt[40];
static ucontext_t cxt_main;
static int inital = 0;
my_pthread_t* MTH;


int main(int argc, char const *argv[])
{
	 MTH = malloc(sizeof(my_pthread_t));
	 init((void*)MTH_Runner);

	my_pthread_t* thread1 = malloc(sizeof(my_pthread_t));
	my_pthread_create(thread1,NULL,(void*)&MTH_Runner,NULL);

	my_pthread_t* thread2 = malloc(sizeof(my_pthread_t));
	my_pthread_t* thread3 = malloc(sizeof(my_pthread_t));

	my_pthread_create(thread2,NULL,(void*)fun,NULL);
	my_pthread_create(thread3,NULL,(void*)fun1,NULL);
	
	 swapcontext(&cxt_main,&cxt[0]);
	
	return 0;
}
//function for threads to run 
void* fun()
{
	my_pthread_join(2,NULL);
	printf("FUN\n" );
	my_pthread_exit(NULL);
}
//function for threads to run
void* fun1()
{
	
		printf("STUFF\n" );
	my_pthread_exit(NULL);
}

void init(void* Funs){

  MTH->size = 0;
  MTH->tid = inital;
  inital++;  
  MTH->next = NULL;
}

my_pthread_t* runT(my_pthread_t* head)
{
	my_pthread_t* start = head;
	while(start->next!=NULL)
		start = start->next;
	return start;
}

void Enqueue(my_pthread_t* input)
{
	my_pthread_t* ending = runT(MTH);    //Find end of ArrayList
	ending->next = input;       //Insertion of newThread
	input->next = NULL;
	MTH->size=MTH->size+1;
}

void Dequeue(int Remove_tid)
{
  if(Remove_tid == 0)
  {
    return;
  }
	my_pthread_t * head = MTH; //Local variable used to manipulate the MTH securely
	my_pthread_t* prev = NULL; 
	my_pthread_t * temp;
while (head != NULL) {
//While head is not null
		if (head->tid == Remove_tid) {
			temp = head; //temp takes on the same values as head
			prev->next = head->next; //the next thread in prev gets defined as next thread in head 
			head->next = NULL; //the next thread in head is set to null
			head = prev->next; //heads current space is set to prev->next, ie the next thread in head 
		}

		else {
			prev = head;//prev takes the value of head
			head = head->next; //head takes the value of the next thread in its space
		}

	}
  MTH->size = MTH->size-1;


  
}  
//initializing a thread in our scheduler
void initThread(my_pthread_t * lead){
	lead->tid = inital-1;
	lead->t_priority = 0;
	lead->state = READY;
	lead->t_context = cxt[inital-1];
	lead->next = NULL;
	inital++;
}
//printing the current order of the thread queue
void printQueue(my_pthread_t* head) {
	printf("%d, ", head->tid);
	while (head->next != NULL) {
		head = head->next;
		printf("%d, ", head->tid);
	}
	printf("\n");
}
int my_pthread_create(my_pthread_t* thread, pthread_attr_t* attr, void *(*function)(void*), void* arg)
{
		//if the total amount of threads is greater than our max, in this case 38 than return a -1
    if(inital == 39 )
    {
    	return -1;
    }
  	//if this is the first thread, allocate space for and initialize the context of the MASTER THREAD HANDLER
    if (inital == 1)
    {
    	char* func1_stack = (char*)malloc(sizeof(char)*16384);
 		getcontext(&cxt[0]);
 		cxt[0].uc_stack.ss_sp = func1_stack;
    	cxt[0].uc_stack.ss_size = 16384;
    	 	
		cxt[0].uc_link = &cxt_main;
 
 		makecontext(&cxt[0] , (void*)function,0);
		MTH->t_context = cxt[0];
		inital++;
		return 1;
		 	 		 
 	}
  //Otherwise make a normal thread
	else
	{
 		getcontext(&cxt[inital-1]);
 		cxt[inital-1].uc_stack.ss_sp = (char*)malloc(sizeof(char)*16384);
    	cxt[inital-1].uc_stack.ss_size = 16384;
    	 	
		cxt[inital-1].uc_link = &cxt[0];
 
 		makecontext(&cxt[inital-1] , (void*)function,0);
		initThread(thread);
		Enqueue(thread);
		return 1;
	}
}
//Function of the master handler to monitor and change the statuses of all other threads
void MTH_Runner()
{
	my_pthread_t * temp;
	while(MTH->size >=1 )
	{
		
		temp = MTH;
			printQueue(MTH);

			
		while(temp->state != READY && temp->next!=NULL )
		{
			if(temp->state == WAITED)
			{
				temp->state = READY;
			}
			temp = temp->next;
		}

					printf("%d\n", temp->tid);	
			temp->state = RUNNING;
			MTH->current = temp;
		setcontext(&(temp->t_context));
		
	}	

}
void my_pthread_yield()
{	
	MTH->current->state = WAITED; //The current thread status is set to wait
	swapcontext(&((MTH->next)->t_context),&cxt[0]); //Saves the current context of the next thread and then points to the MTH
}

void my_pthread_exit(void* value_ptr)
{
  Dequeue(MTH->current->tid); //Dequeues the current thread
  free(MTH->current); //frees the memory space that thread occupied
  MTH->current = NULL; //Initializes that space to 0
  setcontext(&cxt[0]); //Returns the thread back to the MTH
}

int my_pthread_join(int tid, void** value_ptr)
{
	my_pthread_t * temp;	
	temp = MTH;
		while( temp->tid != tid && temp->next!=NULL )
		{
			temp = temp->next;
		}
		if(temp->tid == tid)
		{
			my_pthread_yield();
		}
		else
		{
			return 0;
			
		}
  return -1;
}