#include <sys/types.h>
#include "treeTest.c"

#define FILE_NAME "pt.txt"

struct tree_node* makeNode(struct processedLine *pline, struct processedLine **entryList, int numLines);
void forkAndSleep(struct tree_node* root);

struct tree_node* makeNode(struct processedLine *pline, struct processedLine **entryList, int numLines)
{
	//base case leaf node
	
	printf("%s\n", pline->name);
	
	if(pline->numChildren == 0)											//if this node has no children, it is a leaf node
	{
		printf("got in if\n");
		
		struct tree_node *leaf = (struct tree_node *) calloc((size_t) 1, sizeof (struct tree_node));
		
		printf("calloced\n");
		
		leaf->children = NULL;
		leaf->name = pline->name;
		leaf->numChildren = 0;
		printf("Finished constructing leafffffffff. Ready 2 preent?\n");
		return leaf;
	}
	else
	{
		int i,j;
		struct tree_node *node = (struct tree_node *) calloc((size_t) 1, sizeof (struct tree_node));
		node->name = pline->name;
		node->numChildren = pline->numChildren;
		
		printf("got here 1 \n");
		
		node->children = (struct tree_node **) calloc((size_t) pline->numChildren, sizeof(struct tree_node*));
		for(i = 0; i < node->numChildren; i++)
		{
			printf("got here outer loop \n");
			char *temppLineChildName = pline->childrenNames[i];
			struct processedLine *temppLine = NULL;
			//Search entryList for struct processedLine whose name matches childrenNames[i]
			for(j = 0; j < numLines; j++)
			{
				printf("got here inner loop\n");
				struct processedLine *currentProcessedLine = entryList[j];
				printf("got here currentProcessedLine \n");
				printf("got here numLines = %d \n", numLines);
				printf("currentProcessedLine->numChildren = %d \n", j);
				char *tempName = currentProcessedLine->name;
				printf("got here tempName \n");
				
				printf("tempName: %s\ntemppLineChildName: %s\n", tempName, temppLineChildName);
				
				if(strcmp(tempName,temppLineChildName) == 0)
				{
					printf("got here if strcmp \n");
					temppLine = currentProcessedLine;
					printf("got here break \n");
					break;
				}
			}
			printf("got out of loop \n");
			node->children[i] = makeNode(temppLine, entryList, numLines);
			printf("%s\n", node->children[i]);
			
		}
		return node;
	}
}

void print_tree(struct tree_node* root)//output format needs to be fixed later
{
	if(root->numChildren == 0)
	{
		printf("%s\t", root->name);
		return;
	}
	else
	{
		int i;
		printf("%s\n", root->name);
		for(i = 0; i < root->numChildren; i++)
		{
			struct tree_node *currentChild = root->children[i];
			print_tree(currentChild);
		}
	}
	//printf("\n");
}

void read_tree_file(const char *filename)
{
	//Setup
	int numLines = 0;
	int i = 0;															//Loop counter
	char current;														//Current character being looked at
	char *entryList = (char*) calloc((size_t) 50, sizeof(char));		//Allocate space for entries in tree file
	FILE *fp = fopen(FILE_NAME, "r");									//Open tree file
	
	
	while(fscanf(fp, "%c", &current) != EOF)								//Loop until end of file; fscanf reads the file
																			//	according to a specified format
	{
		if(current == '\n')													//Keep newline characters and spaces out of array
		{
			numLines++;
			continue;
		}
		else if(current == ' ')
		{
			continue;
		}
		else
		{
			entryList[i] = current;											//Place current character being looked at in file into
																			//	character array entryList
			i++;
		}
	}
	numLines -= 1;		//This holds if the file ends with a newline character
	//this ** returns an array of (struct processedLine*)'s, NOT a pointer to them
	struct processedLine ** treeLine = processLines(entryList, numLines);
	printf("%s\n", entryList);
	struct tree_node* parentNode = makeNode(treeLine[0], treeLine, numLines);
	forkAndSleep(parentNode);
	print_tree(parentNode);

	return;
}

void forkAndSleep(struct tree_node* root)
{
	if(root->numChildren == 0)
	{
		sleep(1);
	}
	else
	{
		int i;
		for(i = 0; i < root->numChildren; i++)
		{
			struct tree_node* currentChild = root->children[i];
			pid_t pid = fork();
			if(pid < (pid_t) 0)
			{
				printf("Fork failed\n");
			}
			else if(pid == (pid_t) 0)	//Child process
			{
				printf("Child process pid: %d\n", getpid());
				forkAndSleep(currentChild);
			}
			else 						//Parent process
			{
				printf("This is the parent process with pid: %d\n", getpid());
				wait(NULL);
				printf("%s\n", currentChild->name);
			}
		}
	}
}

int main()	//allow input from command line later
{
	read_tree_file(FILE_NAME);
	return 0;
}

//Go through array elements	BACKWARDS (to do recursively)...
	
	//nodeCount = number of nodes specified by a number.
	//	This value will be used to determine how to read the next
	//	number of elements in the array
	
	//KEEP IN MIND THIS MUST BE RECURSIVE
	
	//Consider putting a number at the beginning of the file,
	//	specifying the number of nodes in the tree so that you can abort
	//	if too large
	
	//Let letter mean any character that is not a number (since node names can be anything)
	//if element is a letter and nodeCount == 0
	//	This is the current node being looked at
	//else if element is a number
	//	if the number != 0
	//		This is the number of children to read for the current node
	//	else the number == 0
	//		This is the last child node in the branch. Call sleep()
	//else if element is a letter and nodeCount > 0
	// The element is a child of the first element in the line
