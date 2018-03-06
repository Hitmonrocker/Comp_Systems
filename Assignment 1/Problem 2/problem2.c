/*Including libraries and the tree code I wrote. This has the same
 * effect as taking all of the code in treeTest.c and putting it here.
 * These included files MUST NOT HAVE MAIN FUNCTIONS!
*/
#include <sys/types.h>
#include "treeTest.c"

#define FILE_NAME "pt.txt"		//defining a constant for later use

//function declarations (required)
struct tree_node* makeNode(struct processedLine *pline, struct processedLine **entryList, int numLines);
void forkAndSleep(struct tree_node* root, int fdPipe[]);

//********************************************************************//
//functions written here. main() is at the very bottom.

/*function to make a tree node. See treeTest.c for pLine and entryList.
 * numLines is obtained in the function read_tree_file (see below),
 * where this function is used.
*/
struct tree_node* makeNode(struct processedLine *pline, struct processedLine **entryList, int numLines)
{
	//This is a recursive function
	
	//base case leaf node
	if(pline->numChildren == 0)											//if this node has no children, then it is a leaf node
	{
		//recall that a "\" character is for line continuation
		struct tree_node *leaf = (struct tree_node *)\
			calloc((size_t) 1, sizeof (struct tree_node));				//allocate memory for leaf node
				
		//set leaf members
		leaf->children = NULL;
		leaf->name = pline->name;
		leaf->numChildren = 0;
		return leaf;
	}
	
	//case: not a leaf node
	else
	{
		int i,j;
		struct tree_node *node = (struct tree_node *)\
			calloc((size_t) 1, sizeof (struct tree_node));				//memory allocation for node
			
		//setting members of node
		node->name = pline->name;
		node->numChildren = pline->numChildren;
		node->children = (struct tree_node **)\
			calloc((size_t)pline->numChildren, sizeof(struct tree_node*));
		
		//Not the base case
		for(i = 0; i < node->numChildren; i++)							//loop through all of the current node's children
		{
			char *temppLineChildName = pline->childrenNames[i];			//temporary string to store current child name
			struct processedLine *temppLine = NULL;
			
			//Search entryList for struct processedLine whose name matches childrenNames[i]
			for(j = 0; j < numLines; j++)
			{
				struct processedLine *currentProcessedLine = entryList[j];	//recall that entryList holds an array of all the processed lines
				char *tempName = currentProcessedLine->name;	

				if(strcmp(tempName,temppLineChildName) == 0)
				{
					temppLine = currentProcessedLine;
					break;
				}
				
			}
			node->children[i] = makeNode(temppLine, entryList, numLines);
			
		}
		return node;
	}
}
//NOTE: You must print a newline character manually after using this function.
//	print_tree does not print a newline after its output.
void print_tree(struct tree_node* root)
{
	//case: leaf node
	if(root->numChildren == 0)
	{
		printf("%s,", root->name);
		return;
	}
	//case: not a leaf node
	else
	{
		int i = 0;
		printf("%s(", root->name);
		//for(i = 0; i < root->numChildren; i++)
		do
		{
			struct tree_node *currentChild = root->children[i];
			print_tree(currentChild);
			i++;
			if(i == root->numChildren)
			{
				printf(")");
			}
		}
		while(i < root->numChildren);
	}
}

struct tree_node* read_tree_file(const char *filename)
{
	//Setup
	int numLines = 0;
	int i = 0;															//Loop counter
	char current;														//Current character being looked at
	char *entryList = (char*) calloc((size_t) 50, sizeof(char));		//Allocate space for entries in tree file
	FILE *fp = fopen(FILE_NAME, "r");									//Open tree file
	
	
	while(fscanf(fp, "%c", &current) != EOF)							//Loop until end of file; fscanf reads the file
	{																	//	according to a specified format
	
		if(current == '\n')												//Keep newline characters and spaces out of array
		{
			numLines++;													//count newlines to count number of nodes
			continue;
		}
		else if(current == ' ')
		{
			continue;
		}
		else
		{
			entryList[i] = current;										//Place current character being looked at into
																		//	character array entryList
			i++;
		}
	}
	numLines -= 1;														//This is needed if the file ends with a newline character
	
	struct processedLine ** treeLine = processLines(entryList, numLines);	//treeLine is an array of processedLines
	
	for(i = 0; i < numLines; i++)
	{
		struct processedLine* treeCurrentLine = treeLine[i];
	}
	
	struct tree_node* parentNode = makeNode(treeLine[0], treeLine, numLines);	/*	parentNode is the very first node in the file with 
																				 *	an entryList of all its children. The number of
																				 *	children that the parent has is the same as the
																				 *	number of lines in the file, excluding the first
																				*/
	int* fdPipe = (int*) calloc((size_t) 2, sizeof(int));
	forkAndSleep(parentNode, fdPipe);
	print_tree(parentNode);
	printf("\n");

	return parentNode;
}

//forkAndSleep is a recursive function
void forkAndSleep(struct tree_node* root, int fdPipe[])
{
	int i;
	if(root->numChildren == 0)
	//this is a last child node (at the bottom of the tree)
	{
		struct tree_node* currentChild = root->children[i];
		char* buf = (char*) calloc((size_t) 50, sizeof(char*));
		write(fdPipe[1], buf, 50);
		sleep(.1);
	}
	else
	{
		int sk;
		int ** arrFd = calloc((size_t) root->numChildren, sizeof(int*));
		
		for(sk = 0; sk < root->numChildren; sk++)
			{
				int temp[2] = {0, 0};
				arrFd[sk] = (int*) &temp;
			}
			

		for(i = 0; i < root->numChildren; i++) 							//loop goes through all children of current node
		{
			struct tree_node* currentChild = root->children[i];
			
			pipe(arrFd[i]);												//pipes for children
			
			if(pipe(arrFd[i]) < 0)
			{
				printf("Error piping\n");
				exit(0);
			}
			close(arrFd[i][0]);
			
			pid_t pid = fork();
			
			if(pid < (pid_t) 0)
			{
				printf("Fork failed\n");
			}
			else if(pid == (pid_t) 0)	//Child process
			{
				char *buf = (char*) calloc((size_t) 50, sizeof(char));
				write(arrFd[i][1], &buf, 50);
				forkAndSleep(currentChild, arrFd[i]);
			}
			else 						//Parent process
			{
				/*since the first child that the parent forks will recursively
				 * fork until it gets to the last child, the parent will
				 * wait until all the children are done
				*/
				char *buf = (char *) calloc((size_t) 50, sizeof(char));
				read(arrFd[i][0], &buf, 50);
				wait(NULL);
			}
		}
	}
}

int getChildPids(struct tree_node* child)
{
	
}

int main()	//allow input from command line later
{
	struct tree_node* parentNode;
	printf("\n");
	parentNode = read_tree_file(FILE_NAME);
	printf("\n\nparentNode: %s\n", parentNode->name);
	return 0;
}
