#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <fcntl.h>      
#include <sys/stat.h>
#include <sys/wait.h>
#include <semaphore.h>
#define  SEM_NAME "semi"

int main(int argc, char const *argv[])
{
	
	 sem_t *semaphore = sem_open(SEM_NAME, O_RDWR);

	 sem_wait(semaphore);
	 printf("I am here -%d- \n",getpid());

	 printf("Bye -%d- \n",getpid());
	 sem_post(semaphore);

	return 0;
}