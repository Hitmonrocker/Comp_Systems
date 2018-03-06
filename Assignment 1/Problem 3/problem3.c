#include "problem2.c"
#include <signal.h>
//#include "treeTest.c"

void wait_for_children(pid_t pid);
void explain_wait_status(pid_t pid, int status);
pid_t* var;

void sigUSR_Handler(int signum)
{
	printf("Received signal %d\n Wait for Child", signum);
		return;
	//wait_for_children("The current node");
	//	We need to search through the array list to find this
	//	node in it to continue forward. We have this nodes 
	//	PID with getpid() so we just have to search through
	//	the arraylist.
	//
	//
}
void sigKill_Handler(int signum)
{
	printf("Received signal %d\n ", signum);
		return;
}
/*void sigCont_Handler(int signum)
{
	printf("Received signal %d\n", signum);
		return;
}*/

int main() 
{
	int status;
	signal(SIGUSR1, sigUSR_Handler);
	struct tree_node* parentNode;
	printf("\n");
	parentNode = read_tree_file(FILE_NAME);
	printf("\n\nparentNode: %s\n", parentNode->name);
	return 0;
}
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
 void wait_for_children(pid_t pid){
	int i = 0;
	int status;
	waitpid(pid, &status, 0);
	explain_wait_status(pid,status);
	/*for(i < root->numChildren; i++)
		{
			struct tree_node *currentChild = root->pidArr[i];			     
			wait_for_children(currentChild);
			kill(currentChild->pid, SIGKILL);
			explain_wait_status(pid, status);
		
		}*/
	}
