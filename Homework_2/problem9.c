#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <semaphore.h>
  #define _GNU_SOURCE 
void Teacher();
void Child();
void Parent();
sem_t TeacherC;
sem_t ParentC;
sem_t ChildC;
int main(int argc, char const *argv[])
{
	int N = 10;
	int R = 4;
	int T = R/2;
	int P = N-R-T;
	size_t size = N;
	sem_init(&TeacherC,T,0);
	sem_init(&ChildC,R,0);
	sem_init(&ParentC,0,P);
	pthread_t t[size];
	int i = 0;
	pthread_create(&t[0],NULL,(void*)*Child,NULL);
	pthread_create(&t[1],NULL,(void*)*Child,NULL);
	pthread_create(&t[2],NULL,(void*)*Child,NULL);
	pthread_create(&t[3],NULL,(void*)*Teacher,NULL);
	pthread_create(&t[4],NULL,(void*)*Parent,NULL);
	pthread_create(&t[5],NULL,(void*)*Teacher,NULL);
	pthread_create(&t[6],NULL,(void*)*Child,NULL);
	pthread_create(&t[7],NULL,(void*)*Parent,NULL);
	pthread_create(&t[8],NULL,(void*)*Parent,NULL);
	pthread_create(&t[9],NULL,(void*)*Parent,NULL);
for (int i = 0; i < size; i++)
	{
		pthread_join(t[i],NULL);
	}

	sem_destroy(&TeacherC);
	sem_destroy(&ChildC);
	sem_destroy(&ParentC);
	
	return 0;
}
void go_home()
{

	printf("I am leaving -%ld-\n",syscall(SYS_gettid));
	pthread_exit(NULL);
}

//-------------------------------\\

void teacher_enter()
{
	sem_post(&TeacherC);
}
void teach()
{
	printf("I am Teaching -%ld-\n",syscall(SYS_gettid));
	sleep(5);
}
void teacher_exit()
{	
	int Cvalue,Tvalue;

	sem_getvalue(&ChildC,&Cvalue);
	sem_getvalue(&TeacherC,&Tvalue);
	if(Cvalue/2 < Tvalue)
	{
		sem_wait(&TeacherC);
		go_home();
	}
	else
	{
		return;
	}
}	


//-------------------------------\\

void child_enter()
{
	sem_post(&ChildC);
}
void learn()
{
	printf("I am learning -%ld-\n",syscall(SYS_gettid));
	sleep(5);
}

void child_exit()
{
	sem_wait(&ChildC);
	go_home();
}

//--------------------------------\\

void parent_enter()
{
	sem_post(&ParentC);
}
void verify_compliance()
{
	int Cvalue,Tvalue;
	printf("I am adult doing adult things -%ld-\n",syscall(SYS_gettid));
	sem_getvalue(&ChildC,&Cvalue);
	sem_getvalue(&TeacherC,&Tvalue);
	if(Cvalue/2 >= Tvalue)
	{
		printf("Regulation is Met\n");
	}

	sleep(5);
}
void parent_exit()
{

	sem_post(&ParentC);
	go_home();
}

//-------------------------------\\





void Teacher()
{
 while(1==1)
 {
 	teacher_enter();
 	teach();
 	teacher_exit();
 }
}


void Child()
{
 while(1==1)
 {
 	child_enter();
 	learn();
 	child_exit();
 }
}



void Parent()
{
 while(1==1)
 {
 	parent_enter();
 	verify_compliance();
 	parent_exit();
 }
}