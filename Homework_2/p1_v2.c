/*
currently running program on Ubuntu for Windows with:
gcc p1.c -lpthread -lm
./a.out
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>


void *fibCalc(void *fibStruct);

struct fibArrPtr{
	int n;
	unsigned long *fibArr;
	unsigned long currentFib;
	unsigned long prevFib;
	unsigned long prevPrevFib;
	pthread_mutex_t *mutexArr;
};

pthread_mutex_t lock;

//This function is for the spawned POSIX thread to run.
//	It calculates the Fibonacci sequence accurately up to the 78th term.
//	Errors are introduced in the 79th term and persist onwards
//	due to the computer representation of floating point numbers.
void *fibCalc(void *fibStruct)
{
	//initialize second thread's variables from fibArrPtr struct
	struct fibArrPtr *fibAP = (struct fibArrPtr *) fibStruct;
	int n = fibAP->n;
	unsigned long *fibArr = fibAP->fibArr;

	fibArr[0] = 0;
	fibArr[1] = 1;

	
	for(int j = 2; j < n; j++)
	{
		//printf("not there\n");
		pthread_mutex_init(&fibAP->mutexArr[j], NULL);
		pthread_mutex_lock(&fibAP->mutexArr[j]);
		//("got here\n");
		fibArr[j] = fibArr[j-1] + fibArr[j-2];
		pthread_mutex_unlock(&fibAP->mutexArr[j]);
	}
	
		//printf("Second half finished\n");
	return NULL;
}


int main(int argc, char **argv)
{
	int n;
	char *input = calloc((size_t) 8, (size_t) sizeof(char));
	printf("Enter a positive integer: ");
	fgets(input, 6, stdin);
	n = atoi(input);										//Convert user input to integer for use in threads
	unsigned long currentFib = 0;

	//check validity of user input
	if(input[0] < '0' || input[0] > '9' || n < 0)
	{
		printf("Error: Input is not a valid number.\n");
		exit(0);
	}
	//got and validated n for number of Fibonacci sequence terms
	
	unsigned long *fibArr = calloc((size_t) n, (size_t) sizeof	(unsigned long));			//Create array to hold Fibonacci numbers
	pthread_mutex_t *mutexArr = calloc((size_t) n, (size_t) sizeof(pthread_mutex_t));		//Create array of mutex locks

	if(n == 0)		//edge case
	{
		printf("%d\n", 0);
	}
	else if(n == 1)		//edge case
	{
		printf("%d\t%d\n", 0, 1);
	}
	else
	{
		struct fibArrPtr fibAP;								//Declare "fibonacci array pointer" structure

		//Initialize structure members
		fibAP.n = n;
		fibAP.fibArr = fibArr;
		fibAP.currentFib = currentFib;
		fibAP.fibArr[0] = 0;
		fibAP.fibArr[1] = 1;
		fibAP.mutexArr = mutexArr;

		struct fibArrPtr *fibptr = calloc((size_t) 1, (size_t) sizeof(struct fibArrPtr));
		fibptr = &fibAP;
		
		//initialize mutex lock
		if (pthread_mutex_init(&lock, NULL) != 0)
	    {
	    	printf("\n mutex init failed\n");
	    	return 1;
	    }

		pthread_t splitter;
		if(pthread_create(&splitter, NULL, fibCalc, fibptr))
		{
			fprintf(stderr, "Error creating thread\n");
			return 1;
		}
		else
		{
			int currentIndex = 0;
			int i = 0;
			while(1)
			{
				//print Fibonacci sequence
				if(pthread_mutex_trylock(&fibAP.mutexArr[currentIndex]) == 0)
				{
					if(i % 10 == 0)
					{
						printf("\n");
					}
					printf("%lu\t", fibAP.fibArr[currentIndex]);
					currentIndex++;
					i++;
					if(currentIndex == n)
					{
						break;
					}
					pthread_mutex_unlock(&fibAP.mutexArr[currentIndex]);
				}
			}
			printf("\n");
		}		
	}
	pthread_mutex_destroy(&lock);
	return 0;
}