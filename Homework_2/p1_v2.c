/*
currently running program on Ubuntu for Windows with:
gcc p1.c -lpthread -lm
./a.out
*/

//NOTE: This program can only calculate up to the 78th Fibonacci number accurately

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>


void *fibCalc(void *fibStruct);

struct fibArrPtr{
	int n;
	double *fibArr;
};

//This function is for the spawned POSIX thread to run.
//	It calculates the Fibonacci sequence accurately up to the 78th term.
//	Errors are introduced in the 79th term and persist onwards
//	due to the computer representation of floating point numbers.
void *fibCalc(void *fibStruct)
{
	//initialize second thread's variables from fibArrPtr struct
	struct fibArrPtr *fibAP = (struct fibArrPtr *) fibStruct;
	int n = fibAP->n;
	double *fibArr = fibAP->fibArr;

	fibArr[0] = 0.0;
	fibArr[1] = 1.0;

	//calculate latter half of Fibonacci numbers and put into Fibonacci array shared with parent
	for(int j = 2; j < n; j++)
	{
		fibArr[j] = round(fibArr[j-1] + fibArr[j-2]);
	}
		//printf("Second half finished\n");
	return NULL;
}


int main(int argc, char **argv)
{
	int n;
	char *input = calloc((size_t) 8, (size_t) sizeof(char));
	printf("Enter a positive integer less than or equal to 1474: ");
	fgets(input, 6, stdin);
	n = atoi(input);										//Convert user input to integer for use in threads

	//check validity of user input
	if(input[0] < '0' || input[0] > '9' || n < 0 || n > 1474)
	{
		printf("Error: Input is not a valid number.\n");
		exit(0);
	}
	//got and validated n for number of Fibonacci sequence terms
	
	double *fibArr = calloc((size_t) n, (size_t) sizeof(double));			//Create array to hold Fibonacci numbers

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

		struct fibArrPtr *fibptr = calloc((size_t) 1, (size_t) sizeof(struct fibArrPtr));
		fibptr = &fibAP;
		
		pthread_t splitter;
		if(pthread_create(&splitter, NULL, fibCalc, fibptr)) {
			fprintf(stderr, "Error creating thread\n");
			return 1;
		}
		else
		{
			/* wait for the second thread to finish */
			if(pthread_join(splitter, NULL)) {
				fprintf(stderr, "Error joining thread\n");
				exit(0);
			}
			else{
				printf("Fibonacci sequence up to term %d:\n", n);
				//print Fibonacci sequence
				for(int i = 0; i < n; i++)
				{
					if(i % 10 == 0)
					{
						printf("\n");
					}
					printf("%.0f\t", fibArr[i]);
				}
				printf("\n");
			}
		}		
	}
	
	return 0;
}
