#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <semaphore.h>

void employee();
void clearner();
enum{false = 0,true = 1 };

sem_t in_office;

int main(int argc, char const *argv[])
{
	
	sem_init(&in_office,0,1);
	size_t size = 2;
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
	sem_destroy(&in_office);
	
	return 0;
}

void working_window()
{
	printf("Working on window -%ld-\n",syscall(SYS_gettid));
}
void clean_office()
{
	printf("cleaning window -%ld-\n",syscall(SYS_gettid));
}
void break_time()
{
	printf("Taking a Break -%ld-\n",syscall(SYS_gettid));
	sleep(5);
	pthread_exit(NULL);
}

void employee()
{
	while(true==1)
	{
		
		sem_wait(&in_office);
		working_window();
		sem_post(&in_office);
		break_time();
	}

}
void clearner()
{
	while(true==1)
	{

		sem_wait(&in_office);
		clean_office();
		sem_post(&in_office);
		break_time();
	}

}