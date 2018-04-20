//.h file
#ifndef Simple_SCH_H
#define Simple_SCH_H
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <ucontext.h>
#include <errno.h>
#include <limits.h>
#include <sys/time.h>
#include <string.h>
#include <limits.h>
#include <math.h>

//States that the thread can be in 
typedef enum t_state{
  NEW,
  READY,
  RUNNING,
  TERMINATED,
  WAITED,
  BLOCKED,
}t_state;
//Thread Structure 
typedef struct _tcb {
  int tid;
  int state;
  int size;
  int joinid;
  int t_priority;
  ucontext_t t_context;
  struct _tcb* next;
  struct _tcb* current;
} my_pthread_t;


int my_pthread_create(my_pthread_t* thread, pthread_attr_t* attr, void *(*function)(void*), void* arg);

void my_pthread_yield();

void my_pthread_exit(void* value_ptr);

int my_pthread_join(int tid, void** value_ptr);

my_pthread_t* runT(my_pthread_t* head);

void Enqueue(my_pthread_t* input);

void initThread(my_pthread_t * lead);

void Dequeue(int Remove_tid);

void printQueue(my_pthread_t* head);

void init( void* Funs);

void* fun();

void* fun1();

void MTH_Runner();

#endif
