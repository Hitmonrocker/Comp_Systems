#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <semaphore.h>
  #define _GNU_SOURCE 
void employee();
void clearner();
enum{false = 0,true = 1 };

sem_t waits;
sem_t in_office_C;
sem_t in_office_E;
static int cleaning = 1;
int main(int argc, char const *argv[])
{
	int N = 10;
	size_t size = N;
	sem_init(&waits,0,1);
	sem_init(&in_office_E,0,5);
	sem_init(&in_office_C,0,5);

	pthread_t t[size];
	for (int i = 0; i < size; i++)
	{
		if(i%2==0)
		{	
			pthread_create(&t[i],NULL,(void*)*employee,NULL);
		}
		else
		{
			pthread_create(&t[i],NULL,(void*)*clearner,NULL);
		}
	}

for (int i = 0; i < size; i++)
	{
		pthread_join(t[i],NULL);
	}

	sem_destroy(&in_office_E);
	sem_destroy(&in_office_C);
	sem_destroy(&waits);
	
	return 0;
}

void working_window()
{
	printf("Working on window -%ld-\n",syscall(SYS_gettid));
	pthread_yield(NULL);
}
void clean_office()
{
	printf("cleaning window -%ld-\n",syscall(SYS_gettid));
	pthread_yield(NULL);
}
void break_time()
{
	int value;
		
	sem_wait(&waits);
		sem_getvalue(&in_office_E,&value);
		if(value == 5)
			cleaning = -1;
		else
			cleaning = 1;
		sem_post(&waits);
	pthread_exit(NULL);
}


void employee()
{
	while(true==1)
	{
	int value;
		sem_wait(&waits);
		sem_getvalue(&in_office_C,&value);
		if(value == 5)
			cleaning = 1;
		else
			cleaning = -1;
		sem_post(&waits);
		while(cleaning<0)
		{
		}
		
			sem_wait(&in_office_E);
		working_window();
		printf("Taking a Break -%ld-\n",syscall(SYS_gettid));
		sem_post(&in_office_E);
		break_time();
		
	}

}
void clearner()
{
	while(true==1)
	{
	int value;
		sem_wait(&waits);
		sem_getvalue(&in_office_E,&value);
		if(value == 5)
			cleaning = -1;
		else
			cleaning = 1;
		sem_post(&waits);
		while(cleaning>0)
		{

		}
			sem_wait(&in_office_C);
		clean_office();
				printf("Taking a Break -%ld-\n",syscall(SYS_gettid));
			sem_post(&in_office_C);
		break_time();
	}
}