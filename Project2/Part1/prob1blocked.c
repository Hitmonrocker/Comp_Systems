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

static int p;
int mms(int* s, int i, int size);
sigset_t x;//declaring a sigset pointer for the proc mask
sigUSR1_Handler(int signum){
  printf("\n\nReceived signal SIGUSR1 used for process determination.\n");
  p = signum;
    return;
}
void sigUSR2_Handler(int signum){
 printf("\n\nReceived signal SIGUSR2 used for process determination.\n");
 p = signum;
    return;
}
void sigCONT_Handler(int signum){
  printf("\n\nReceived signal SIGCONT used for process determination.\n");
  p = signum;
    return;
}
void sigKILL_Handler(int signum){
  printf("\n\nReceived signal SIGKILL which cannot be blocked.\n");
      return;
}
void sigSTOP_Handler(int signum){
  printf("\n\nReceived signal SIGSTOP which cannot be blocked.\n");
      return;
}
void sigINT_Handler(int signum){
  printf("\n\nReceived signal SIGINT which is being blocked by sigprocmask.\n");
      return;
}
void sigTERM_Handler(int signum){
  printf("\n\nReceived signal SIGTERM which is being blocked by sigprocmask.\n");
      return;
}
void sigQUIT_Handler(int signum){
  printf("\n\nReceived signal SIGQUIT which is being blocked by sigprocmask.\n");
      return;
}
void sigABRT_Handler(int signum){
  printf("\n\nReceived signal SIGABRT which is being blocked by sigprocmask.\n");
      return;
}
int main(int argc, char const *argv[])
{

    sigemptyset(&x);//initializing an empty set of signals to be blocked
    sigaddset(&x, SIGABRT);//adding signal to set
    sigaddset(&x, SIGTERM);
    sigaddset(&x, SIGINT);
    sigaddset(&x, SIGKILL);
    sigaddset(&x, SIGQUIT);
    sigaddset(&x, SIGSTOP);
    sigprocmask(SIG_BLOCK, &x, NULL);//the actual block mask, if something is in x than it is blocked
    time_t t1, t2;
    FILE* FP;
    signal(SIGUSR1, sigUSR1_Handler);//assigning signals to the custom handlers 
    signal(SIGUSR2, sigUSR2_Handler);
    signal(SIGCONT, sigCONT_Handler);
    signal(SIGKILL, sigKILL_Handler);
    signal(SIGSTOP, sigSTOP_Handler);
    signal(SIGQUIT, sigQUIT_Handler);
    signal(SIGINT, sigINT_Handler);
    signal(SIGTERM, sigTERM_Handler);
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
  kill(list[1],SIGTERM);
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
                 raise(SIGKILL);
                }
              else
                {     
                  t1 = time(0);
                  raise(SIGUSR1);   
                  int ret = mms(arr,p,atoi(argv[2]));   
                     t2 = time(0);
                    if(t2-t1 >=3)
                    {
                     
                      raise(SIGQUIT);
                    }
                    else
                    {
                        printf("This is the sum : %d\n",ret);
                    }

                }
              }
          else
            { 
              t1 = time(0);
              raise(SIGUSR2);
           int ret = mms(arr,p,atoi(argv[2]));
              t2 = time(0);
              if(t2-t1 >= 3)
                    {
                      raise(SIGQUIT);
                    }
                    else
                    {
                        printf("This is the Min : %d\n",ret);
                    }
            }
      }
      else
      {
        t1 = time(0);
        raise(SIGCONT);
       int ret = mms(arr,p,atoi(argv[2]));
         t2 = time(0);
        if(t2-t1 >= 3)
          {
            raise(SIGQUIT);
           }
           else
            {
           printf("This is the Max : %d\n",ret);
               }
               
      }
    }
    else
    {
      waitpid(list[3],&status[3],0);
      waitpid(list[1],&status[1],0);
      waitpid(list[2],&status[2],0);
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
    
