#include "problem2.c"
#include <signal.h>
//#include "treeTest.c"

void wait_for_children(pid_t pid);
void explain_wait_status(pid_t pid, int status);
pid_t* var;

//Signal Handler for custom signal sigUSR, which has our wait for children function
void sigUSR_Handler(int signum)
{
	printf("Received signal %d\n Wait for Child", signum);
		return;
}
void sigKill_Handler(int signum)
{
	printf("Received signal %d\n ", signum);
		return;
}

int main() 
{
	int status;  //defining the status for the Signal handlers
	signal(SIGUSR1, sigUSR_Handler);//defining the signal for it's appropriate handler
	//print code from problem 2 to display the tree
	struct tree_node* parentNode;
	printf("\n");
	parentNode = read_tree_file(FILE_NAME);
	printf("\n\nparentNode: %s\n", parentNode->name);
	return 0;
}
//standard function to explain the wait status
explain_wait_status(pid_t pid, int status)
{
	if (WIFEXITED(status))
		fprintf(stderr, "Child with PID = %ld terminated normally, exit stauWWWs = %d\n",
			(long)pid, WEXITSTATUS(status));
	else if(WIFSIGNALED(status))
		fprintf(stderr, "Child with PID = %ld was terminated by a signal, signo = %d\n",
			(long)pid, WTERMSIG(status));
	else if(WIFSTOPPED(status))
		fprintf(stderr, "Child with PID = %ld has been stopped by a signal, signo = %d\n",
			(long)pid, WSTOPSIG(status));
	fflush(stderr);
}
//Custom function to wait for children 
 void wait_for_children(pid_t pid){
 	//defining the status integer for the waitpid function
	int status;
	//parent waits for its children to print creating that depth-first structure
	waitpid(pid, &status, 0);
	//explain why the program was waiting or terminated
	explain_wait_status(pid,status);
	}
