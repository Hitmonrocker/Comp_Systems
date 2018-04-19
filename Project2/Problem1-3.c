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
#include <pthread.h>

void *sumThread(void* arg);
void *maxThread(void* arg);
void *minThread(void* arg);

typedef struct _input
{
  int* array;
  int size;
}input;

static sigset_t   signal_maskFirst;  
int main(int argc, char const *argv[])
{
    FILE* FP;
    
 
    sigaddset (&signal_maskFirst, SIGTERM);
    sigaddset (&signal_maskFirst, SIGABRT);
    sigaddset (&signal_maskFirst, SIGTSTP);
    sigaddset (&signal_maskFirst, SIGKILL);

    
    char *buffer;
    size_t bufsize = 32;
    size_t characters;
    input* in = malloc (sizeof(input)*1);
    buffer = (char *)calloc('\0',bufsize * sizeof(char));
    
    printf("%s\n",argv[1]);
    in->size = atoi(argv[2]);

    int* arr = (int*) malloc(sizeof(int)*(atoi(argv[2])));
        FP = fopen(argv[1],"r");
    
    int counter = 0;

  while(getline(&buffer,&bufsize,FP) != EOF && counter<atoi(argv[2])){
     arr[counter] = atoi(buffer);
      counter++;
    }
    in->array = arr;
  fclose(FP);
  pthread_t sumTh, maxTh, minTh;
 
  pthread_sigmask(SIG_BLOCK, &signal_maskFirst, NULL);
  pthread_create(&minTh, NULL, (void*)minThread, (void*)in);
  

  sigaddset (&signal_maskFirst, SIGINT);
  sigaddset (&signal_maskFirst, SIGQUIT);
  sigaddset (&signal_maskFirst, SIGSTOP);

  pthread_sigmask(SIG_BLOCK, &signal_maskFirst, NULL);

  pthread_create(&sumTh, NULL, (void*)sumThread, (void*)in);
  pthread_create(&maxTh, NULL, (void*)maxThread, (void*)in);
    
  pthread_kill(sumTh,SIGQUIT);

  pthread_join(sumTh,NULL);
  pthread_join(maxTh,NULL);
  pthread_join(minTh,NULL);
  
  
  return 0;
}
void *sumThread(void* arg){
    int sum = 0;  
 int sig_caught;
  int rc = sigwait (&signal_maskFirst, &sig_caught);
    
    input* in = (input*) arg;
    for(int i = 0; i < in->size; i++)
      {
        sum += in->array[i];
      } 
  printf("Sum is:%d\n", sum); 
}
void *minThread(void* arg){
  input* in = (input*) arg;
    int min = INT_MAX;
    for(int i = 0; i < in->size; i++ )
        {
          if(min > in->array[i])
            min = in->array[i];
        }
  printf("Min is:%d\n", min);
}
void *maxThread(void* arg){
   input* in = (input*) arg;
     int max = INT_MIN;
      for(int i = 0; i< in->size; i++ )
        {
          if(max < in->array[i])
            max = in->array[i];
        }
  printf("Max is:%d\n", max);
}
