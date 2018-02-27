#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include<unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>



void genTree(int counter);
void genTreeExample();

int main(int argc, char const *argv[])
{
printf("If you would like to see the example put in number one other wise enter the height of a tree you want to create\n");
			if(atoi(argv[1])!=1)
			{
			genTree(atoi(argv[1]));
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
	printf("A:\n");
	int counter = 1;
	l = fork();

	if(l == 0)
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
		
				pid_t r = fork();
				if(r==0)
				{
					sleep(1);
					kill(getpid(),SIGALRM);
				}

		waitpid(l,&status,0);
		printf("B: status = %d\n",status);
		waitpid(r,&status2,0);
		printf("C: status = %d\n",status2);
	}
}
void genTree(int counter)
{
	int status,status1,status2;
	pid_t l;
	if(counter<0)
	{
	kill(getpid(),SIGPIPE);
	}
	l = fork();

	if(l == 0)
		{			
			sleep(1);	
			genTree(counter-=2);
			kill(getpid(),SIGINT);
		}
	else
	{
		
				pid_t r = fork();
				if(r==0)
				{
					sleep(1);
					genTree(counter-=2);
					kill(getpid(),SIGKILL);
				}
					waitpid(r,&status2,0);
		printf("%d->%d: status = %d\n",getpid(),r,WTERMSIG(status2));

		waitpid(l,&status,0);
		printf("%d->%d: status = %d\n",getpid(),l,status);
	}
}
/*


	

*/