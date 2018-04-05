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

sem_t in_office;
sem_t canWork;
int main(int argc, char const *argv[])
{
	int N = 10;
	size_t size = 2*N;
	sem_init(&in_office,0,N);
	sem_init(&canWork,0,1);
	pthread_t t[size];
	for (int i = 0; i < size; i++)
	{
		if(i!=10)
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
	sem_destroy(&in_office);
	sem_destroy(&canWork);
	
	return 0;
}

void working_window()
{
	printf("Working on window -%ld-\n",syscall(SYS_gettid));
	pthread_yield();
}
void clean_office()
{
	printf("cleaning window -%ld-\n",syscall(SYS_gettid));
	pthread_yield();
}
void break_time()
{
	printf("Taking a Break -%ld-\n",syscall(SYS_gettid));
	pthread_exit(NULL);
}

void employee()
{
	while(true==1)
	{
		
		sem_trywait(&canWork);
		sem_wait(&in_office);
		working_window();
		sem_post(&in_office);
		sem_post(&canWork);
		break_time();
	}

}
void clearner()
{
	while(true==1)
	{
		sem_wait(&canWork);
		clean_office();
		sem_post(&canWork);
		break_time();
	}

}