#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>

int res = 0, num = 2, pd[2], arg[32];			//Declare integers and int arrays. res = final result, num = another final result, pd = pipe descriptor, arg = arguments from threads
pthread_t thread[32];							//Declare array of 32 threads
sem_t sem;										//Declare semaphore

void h(int s)
{
	printf("\t\th Reading num from pipe: %d\n", num);
	read(pd[0], &num, sizeof(num));				//Read the number of bytes needed for an integer from read end of pipe into the the num buffer
	sem_post(&sem);								//Increment (unlock) semaphore
}

void *f(void *var)
{
	printf("\t\tf Running. res initially %d\n", res);
	int tmp = *(int *)var;						//Store passed argument as an integer in temporary variable
	res += tmp;									//Add temporary integer to result
	printf("\tres changed. Now res = %d\n", res);
	pthread_exit((void *) 0);					//Terminate calling thread
}

int main()
{
	int i, k, p, pstatus;						//Initialize integers
	void *tstatus;								//Initialize void pointer

	sem_init(&sem, 0, 0);						//Initialize unnamed semaphore at address of sem, shared
												//	between the threads of a process, with an initial value of 1
	signal(SIGUSR1, h);							//When the child process terminates, run function h
	pipe(pd);									//Initialize pipe

	for(i = 0; i < num; i++)					//Run loop num times
	{
		p = fork();								//Fork a child process
printf("forked child\n");
		if(p == 0)								//If the current process is the child process, run this code
		{
printf("child running, num = %d\n", num);
			//IMPORTANT NOTE: In this loop, num only changes in the child process, which terminates each time. 
			sem_wait(&sem);						//Decrement semaphore
			num++;								//Increment num
printf("child num incremented to num = %d\n", num);
			for(k = 0; k < num; k++)			//Run loop num times
			{
				printf("arg loop %d\n", k);
				arg[k] = k;						//Set kth element of integer array arg[] to k
				pthread_create(&thread[k], NULL, f, (void*)&arg[k]);		//Create a thread at the address of thread[k] that runs function f. thread[] is a pthread array
			}

			for(k = 0; k < num; k++)					//Run loop num times
			{
				printf("pthread_join loop %d\n", k);
				pthread_join(thread[k], &tstatus);		//For each thread that exists, join the thread and store the exit status of the target thread in tstatus
			}
			if(num < 4)									//num should be 3, so this should run.
			{
				printf("got num < 4, num = %d\n", num);
				write(pd[1], &num, sizeof(num));		//Write the child's value of num to the pipe
				exit(res);								//Exit the child process normally, passing the value of res to the parent.
			}
		}

		write(pd[1], &num, sizeof(num));		//Write parent's value of num to the pipe
		signal(SIGUSR1, SIG_IGN);				//If the signal SIGUSR1 is delivered to the parent process, ignore it
		kill(0, SIGUSR1);						//Kill a child process


		for(i = 0; i < num; i++)				//Run this loop num (2) times. num here is still the parent's copy of num, which has not changed since initialization.
		{
			wait(&pstatus);						//Wait on child process
			if(WIFEXITED(pstatus))				//If child process terminated normally, run this block
			{
				printf("\t\tgot to WIFEXITED condition. res initial value: %d\n", res);
				res += WEXITSTATUS(pstatus);	//Add exit status value to result. The exit status value is the argument passed to exit() in the child.
				printf("\tWIFEXITED res new value: %d\n", res);
			}
		}
	}
	printf("Final result1: %d \n", res);		//Print resulting value of global variable res
	read(pd[0], &num, sizeof(num));				//Read from pipe and store the result in num
	printf("Final result2: %d \n", num);		//Print (the pipe's) value of num
	return 0;
}