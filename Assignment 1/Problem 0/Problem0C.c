#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include<unistd.h>
#include <sys/wait.h>

struct funshi{
	int max;
	int min;
	int sum;
};

void withFork(int* array, int size);

int main(int argc, char const *argv[])
{
	FILE* FP;
	char *buffer;
    size_t bufsize = 32;
    size_t characters;

    buffer = (char *)calloc('\0',bufsize * sizeof(char));
    printf("%s\n",argv[1]);
	int* arr = (int*) malloc(sizeof(int)*atoi(argv[2]));
	FP = fopen(argv[1],"r");
		int counter = 0;

	while(getline(&buffer,&bufsize,FP) != EOF && counter<atoi(argv[2])){
		 arr[counter] = atoi(buffer);
			counter++;
		}
		
	fclose(FP);
	free(buffer);
	withFork(arr,counter);
	printf("\n");
	

	return 0;
}

void withFork(int* array,int size)
{
	
	int max = array[0];
	int min = INT_MAX;
	int sum = 0;
	int status;
	int counter = 0;
printf("\nAyyy yo Striki this is with forking\n");	
pid_t pid = fork();

if(pid == 0)
{
	pid_t cpid = fork();
	if(cpid == 0)
	{
		pid_t cpid = fork();
		if(cpid == 0)
		{
				pid_t cpid = fork();
				if(cpid == 0)
				{
					while(counter < size)
						{
						sum+=array[counter];
						counter++;
						}
							printf("My Parent is : %d Obtained by %d Sum : %d\n",getppid(),getpid(),sum);
				}
				else
				{
					waitpid(cpid,&status,0);
				}
		}
		else
		{
			while(counter < size)
			{
				if(array[counter]<min)
				min=array[counter];
				counter++;
			}
			printf("My Parent is : %d Obtained by %d Min : %d\n",getppid(),getpid(),min);
			waitpid(cpid,&status,0);
		}
	}
	else
	{
	while(counter < size)
		{
		if(array[counter]>max)
		max=array[counter];
		counter++;
		}
	printf("My Parent is : %d Obtained by %d Max : %d\n",getppid(),getpid(),max);
		waitpid(cpid,&status,0);
	}
}
else
{
	waitpid(pid,&status,0);
	exit(0);
}

}
/*




*/
