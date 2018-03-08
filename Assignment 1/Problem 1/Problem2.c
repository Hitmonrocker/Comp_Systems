#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include<unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>

void genTreeLeft(int counter);
void genTreeRight(int counter);
void genTree(int counter);
void genTreeExample();

int main(int argc, char const *argv[])
{
		int stat;
		int sum = 0;
printf("If you would like to see the example put in number one otherwise enter the height of a tree you want to create\n");
			if(atoi(argv[1])!=1)
			{
				for (int i = 0; i < atoi(argv[1]); ++i)
				{
					pid_t t = fork();
					if(t==0)
					{
					genTree((int)pow((double)2,(double)i));
					raise(SIGKILL);
					}
					else
					{
					 waitpid(t,&stat,0);
					 sum+=(int)pow(2,i);
					printf("Number of Processes Created in tree %d\n",sum);
					}
						
				}
			
			}
			else
			{
					genTreeExample();
			}
	return 0;
}

void genTreeExample()
{	int status,status1,status2;
	pid_t l,r;
	int counter = 1;
	l = fork();

	if(l == 0)
	{
		r = fork();
		if(r == 0)
				{
			pid_t q = fork();
			if(q == 0)
			{
				sleep(1);
				kill(getpid(),SIGINT);
			}
			else
			{
				
				waitpid(q,&status1,0);
				sleep(1);
				printf("D: status = %d\n",status1);
				kill(getpid(),SIGPIPE);
					
			}
		}
		else
		{
			waitpid(r,&status,0);
			printf("B: status = %d\n",status);
				pid_t n = fork();
				if(n==0)
				{
					sleep(1);
					kill(getpid(),SIGALRM);
				}
				else
				{
					waitpid(n,&status2,0);
					printf("C: status = %d\n",status2);
				}
		}
		kill(getpid(),SIGKILL);
	}
	else
	{
		waitpid(l,&status1,0);
		printf("A: status = %d\n",status1);
	}

}
void genTree(int counter)
{
	if(counter<=1)
	{
		return;
	}
	else
	{
		//printf("%d\n",counter);
		counter/=2;
		genTreeRight(counter);
		genTreeLeft(counter);
	}
}
void genTreeRight(int counter)
{
	int stat;
	pid_t r = fork();
	if(r==0)
	{
		//printf("%d->%d\n",getppid(),getpid());
		genTree(counter);
		//sleep(1);
		raise(SIGKILL);
	}
	else
	{
		waitpid(r,&stat,0);
	}
}
void genTreeLeft(int counter)
{
	int stat;
	pid_t l = fork();
	if(l==0)
	{
		//printf("%d->%d\n",getppid(),getpid());
		genTree(counter);
		//sleep(1);
		raise(SIGKILL);
	}
	else
	{
		waitpid(l,&stat,0);
	}
}

/*


	sleep(1);	
				counter--;
				genTree(counter);
				raise(SIGKILL);

*/