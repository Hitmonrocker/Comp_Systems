#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
static int p;
int mms(int* s, int i, int size);
void sigUSR1_Handler(int signum){
  //printf("\n\nReceived signal %d\n", signum);
  p = signum;
    return;
}
void sigUSR2_Handler(int signum){
 // printf("\n\nReceived signal %d\n", signum);
 p = signum;
    return;
}
void sigCONT_Handler(int signum){
//  printf("\n\nReceived signal %d\n", signum);
  p = signum;
    return;
}

int main(int argc, char const *argv[])
{
    FILE* FP;
    signal(SIGUSR1, sigUSR1_Handler);
    signal(SIGUSR2, sigUSR2_Handler);
    signal(SIGCONT, sigCONT_Handler);
    
    char *buffer;
    size_t bufsize = 32;
    size_t characters;

    buffer = (char *)calloc('\0',bufsize * sizeof(char));
    printf("%s\n",argv[1]);
    int* arr = (int*) malloc(sizeof(int)*(atoi(argv[2])));
        FP = fopen(argv[1],"r");
    int counter = 0;

  while(getline(&buffer,&bufsize,FP) != EOF && counter<atoi(argv[2])){
     arr[counter] = atoi(buffer);
      counter++;
    }
    
  fclose(FP);
  pid_t list[4];
  int status[4];
  
    list[0] = fork();
    if(list[0] == 0)
    {
      list[3] = fork();
      if(list[3] == 0)
      {
            
            list[1] = fork();
             if(list[1]==0)
              {
                  
                  list[2] = fork();
              if(list[2] == 0)
                {
                  kill(getppid(),SIGUSR1);      
                }
              else
                {
                  waitpid(list[2],&status[2],0);
                   printf("This is the Sum : %d\n",mms(arr,p,atoi(argv[2])));
                  kill(getppid(),SIGUSR2);
                }
              }
          else
            { 
              waitpid(list[0],&status[0],0);
              printf("This is the Min : %d\n",mms(arr,p,atoi(argv[2])));
              kill(getppid(),SIGCONT);
            }
      }
      else
      {
      waitpid(list[1],&status[1],0);
      printf("This is the Max : %d\n",mms(arr,p,atoi(argv[2])));
      raise(SIGKILL);
      }
    }
    else
    {

      waitpid(list[3],&status[3],0);
    }
  
  return 0;
}


int mms(int *s,int q,int size)
{
   int max = INT_MIN;
   int min = INT_MAX;
   int sum = 0;
   int i = 0;
  switch(q)
  {
    case 10:
      {
      for(; i < size; i++)
      {
        sum += s[i];
      }
      return sum;
    }
        break;
    case 12:
        {
        for(; i < size; i++ )
        {
          if(min > s[i])
            min = s[i];
        }
        return min;
          }
        break;
    case 18:
       {
        for(; i< size; i++ )
        {
          if(max < s[i])
            max = s[i];
        }
        return max;
      }
        break;
    default:
        break;
  }
}
    
