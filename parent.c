#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <fcntl.h>      
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/wait.h>

#include <errno.h>
#define  SEM_NAME "semi"
#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define INITIAL_VALUE 4
#define CHILD_PROGRAM "./child"


enum{false = 0,true = 1 };

sem_t canWork;

int main(int argc, char const *argv[])
{
	int filedes[2];

if (pipe(filedes) == -1) {
  perror("pipe");
  exit(1);
}
	sem_t* semaphore = sem_open(SEM_NAME,O_CREAT | O_EXCL, SEM_PERMS, INITIAL_VALUE);
	sem_close(semaphore);
	pid_t p[5];
	for (int i = 0; i < 5; ++i)
	 {
	 	if((p[i] = fork())<0)
	 	{
	 		perror("fork(2) failed");
            exit(EXIT_FAILURE);
	 	}
	 	if(p[i] == 0)
	 	{
	 		while ((dup2(filedes[1], STDOUT_FILENO) == -1) && (errno == EINTR)) {}

	 		execl(CHILD_PROGRAM,CHILD_PROGRAM,(char*)NULL);
	 	}
	 } 

for (int i = 0; i < 5; ++i)
	{

char buffer[4096];
while (1) {
  ssize_t count = read(filedes[0], buffer, sizeof(buffer));
  if (count == -1) {
    if (errno == EINTR) {
      continue;
    } else {
      perror("read");
      exit(1);
    }
  } else if (count == 0) {
    break;
  }
}
waitpid(p[i],NULL,0);
	}


	return 0;
}
