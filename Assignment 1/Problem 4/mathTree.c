#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define FILE_NAME "nt.txt"

struct treeArr{
	char **array;
	char **format;
	int size;
};

struct treeNode{
	char *name;
	struct treeNode *leftChild;
	struct treeNode *rightChild;
};

struct oneLine{
  char *curr;
  char *left;
  char *right;
};

struct nodeLabel{
	int currentName;
	int left;
	int right;
};

struct treeStructsArr{
  struct oneLine **array;
  struct nodeLabel **format;
};

struct treeArr *makeTreeArr(FILE *fp);
//makeTreeArr takes a file pointer and returns an array containing the nodes
//	of the tree described by the input file. The tree is organized such that
//	Every three elements forms a parent node and its two children. Note that
//	the input file MUST end with a newline character
struct treeArr *makeTreeArr(FILE *fp)
{
	int i = 0;
	int j, k, numElements;
	numElements = 0;

	int isNumStr = 0;
	int numStrBounds[2];

	char currentChar;
	char *input = (char*) calloc((size_t) 2000, sizeof(char));
	char **entryList = (char**) calloc((size_t) 1000, sizeof(char*));
	struct treeArr *result = (struct treeArr *) calloc((size_t) 1, sizeof(struct treeArr));
	char buf[2] = {'\0', '\0'};

	//read each character from file up to separator character and put into an array
	while(fscanf(fp, "%c", &currentChar) != EOF)
	{
//printf("i is %d, currentChar is %c\n", i, currentChar);
		input[i] = currentChar;
		i++;
    if (currentChar == '~')
    {
        break;
    }
	}
//printf("Got past tilde search ~\n");

	k = 0;
	//make array of strings from file entries
	for(j = 0; j <= i; j++)
	{
		currentChar = input[j];
//printf("currentChar: %c\n", currentChar);

		if(currentChar == ' ' || currentChar == '\n')
		{
			numElements++;
			//case: number to space or newline
			if(isNumStr == 1)
			{
				isNumStr = 0;
				char *cpyBuf = calloc((size_t) (numStrBounds[1] - numStrBounds[0] + 1), sizeof (char));
				char *numString = strncpy(cpyBuf, input+numStrBounds[0], (size_t) (numStrBounds[1] - numStrBounds[0] + 1));
//printf("cpyBuf: %s\n", cpyBuf);
//printf("numString: %s\n", numString);
				entryList[k] = numString;

			}
			k++;
			continue;
		}
		else if(currentChar == '+' || currentChar == '*')
		{
			char *symBuf = calloc((size_t) (2), sizeof (char));
			char *symString = strncpy(symBuf, input+j, (size_t) 2);
			entryList[k] = symString;
//printf("got past +* case\n");
//printf("entryList[%d]: %s\n", k, entryList[k]);
			continue;
		}
		else if(currentChar >= '0' && currentChar <= '9')
		{
			//case: space to number
			if(isNumStr == 0)
			{
				isNumStr = 1;
				numStrBounds[0] = j;
				numStrBounds[1] = j;
				continue;
			}
			//case: number to number
			else
			{
				numStrBounds[1] = j;
			}
		}
		else if(currentChar == '\0')
		{
			printf("Null character reached\n");
		}
		else
		{
			printf("Error reading first block\n");

		}

//printf("entryList[%d]: %s\n", j, entryList[j]);
	}

	result->array = entryList;
	result->size = numElements;

//printf("array[0]: %s\n", result->array[0]);
//printf("size: %d\n", result->size);

	i = 0;
	k = 0;
	int numEnts = 0;
	char **entryList2 = (char**) calloc((size_t) 1000, sizeof(char*));

//read each character from file up to end of file and put into an array
	while(fscanf(fp, "%c", &currentChar) != EOF)
	{
		input[i] = currentChar;
		i++;
    printf("(%d) currentChar is %c\n", i, currentChar);
	}

	//make array of strings from file entries
	for(j = 0; j <= i; j++)
	{
		currentChar = input[j];
//printf("currentChar: %c\n", currentChar);

		if(currentChar == ' ' || currentChar == '\n')
		{
			numEnts++;
			//case: number to space or newline
			if(isNumStr == 1)
			{
				isNumStr = 0;
				char *cpyBuf = calloc((size_t) (numStrBounds[1] - numStrBounds[0] + 1), sizeof (char));
				char *numString = strncpy(cpyBuf, input+numStrBounds[0], (size_t) (numStrBounds[1] - numStrBounds[0] + 1));
//printf("cpyBuf: %s\n", cpyBuf);
//printf("numString: %s\n", numString);
				entryList2[k] = numString;
//printf("entryList2[%d]: %s cw\n", k, entryList2[k]);
				k++;
			}
			continue;
		}
		else if(currentChar == 'x')
		{
			char *symBuf = calloc((size_t) (2), sizeof (char));
			char *symString = strncpy(symBuf, input+j, (size_t) 2);
			entryList2[k] = symString;
//printf("got past +* case\n");
//printf("entryList2[%d]: %s\n", k, entryList2[k]);
      k++;
			continue;
		}
		else if(currentChar >= '0' && currentChar <= '9')
		{
			//case: space to number
			if(isNumStr == 0)
			{
				isNumStr = 1;
				numStrBounds[0] = j;
				numStrBounds[1] = j;
				continue;
			}
			//case: number to number
			else
			{
				numStrBounds[1] = j;
			}
		}
		else if(currentChar == '\0')
		{
			printf("Null character reached\n");
		}
		else
		{
			printf("Error reading second block\n");

		}

//printf("entryList[%d]: %s\n", j, entryList[j]);
	}

	result->format = entryList2;

//printf("array[0]: %s\n", result->array[0]);
//printf("format[0]: %s\n", result->format[0]);
//printf("size: %d\n", result->size);
	return result;
}

struct treeStructsArr *makeStructs(struct treeArr *nodeArray)
{
    char **array = nodeArray->array;
    char **format = nodeArray->format;
    int size = nodeArray->size;

    struct treeStructsArr *result = (struct treeStructsArr *)calloc(
        (size_t)1, sizeof(struct treeStructsArr *));
    struct oneLine **resultArray = (struct oneLine **)calloc(
        (size_t)size, sizeof(struct oneLine *));
    struct nodeLabel **resultFormat = (struct nodeLabel **)calloc(
        (size_t)size, sizeof(struct nodeLabel *));

    //Count in 3s
    int i;
    int j = 0;
//printf("Got here count in 3s\n");
    for (i = 0; i < size; i += 3)
    {
        //Check array
        char *currentName = array[i];
        char *leftChildName = NULL;
        char *rightChildName = NULL;
//printf("Got here check array\n");

        //Check format
        char *currFmtName = format[i];
        char *leftFmtName = NULL;
        char *rightFmtName = NULL;
//printf("Got here check format (%d) %s\n", i, format[i]);

        //Check bounds left child
        if (i + 1 < size)
        {
//printf("Got here check bounds left child\n");
            leftChildName = array[i + 1];
            leftFmtName = format[i + 1];
        }
        //Check bounds right child
        if (i + 2 < size)
        {
//printf("Got here check bounds right child\n");
            rightChildName = array[i + 2];
            rightFmtName = format[i + 2];
        }

        //initialize the structs
        struct oneLine *oneLine = (struct oneLine *)calloc(
            (size_t)1, sizeof(struct oneLine));
        struct nodeLabel *nodeLabel = (struct nodeLabel *)calloc(
            (size_t)1, sizeof(struct nodeLabel));
//printf("Got past initializing all teh structs\n");

        //populate the structs with atoi stuff if necessary
        oneLine->curr = currentName;
        oneLine->left = leftChildName;
        oneLine->right = rightChildName;

        nodeLabel->currentName = atoi(currFmtName);
//printf("Got past first atoi\n");
        if (leftFmtName[0] == 'x')
        {
            nodeLabel->left = -1;
        }
        else
        {
            nodeLabel->left = atoi(leftFmtName);
        }
        if (rightFmtName[0] == 'x')
        {
            nodeLabel->right = -1;
        }
        else
        {
            nodeLabel->right = atoi(rightFmtName);
        }
//printf("Got past populating the structs with atoi stuff\n");
        //add the structs to the final arrays
        resultArray[j] = oneLine;
        resultFormat[j] = nodeLabel;
        j++;
    }
    //add the arrays to the final result
    result->array = resultArray;
    result->format = resultFormat;
//printf("Returning result in makeStructs\n");
    return result;
}

//Done
struct treeNode *makeTree(struct oneLine *entryList, struct nodeLabel *labelList, struct oneLine **globalEntryList, struct nodeLabel **globalLabelList)
{
	struct treeNode *root = (struct treeNode *) calloc((size_t) 1, sizeof(struct treeNode));
	root->name = entryList->curr;

	if(labelList->left == -1 && labelList->right == -1)
	{
    //base case. do not reuse
    //take care of everything necessary for left
		root->leftChild = (struct treeNode *) calloc((size_t) 1, sizeof(struct treeNode));
    root->leftChild->name = entryList->left;
    root->leftChild->leftChild = NULL;
    root->leftChild->rightChild = NULL;

    //take care of everything necessary for right
		root->rightChild = (struct treeNode *) calloc((size_t) 1, sizeof(struct treeNode));
    root->rightChild->name = entryList->right;
    root->rightChild->leftChild = NULL;
    root->rightChild->rightChild = NULL;
	}
	else if(labelList->left == -1 && labelList->right != -1)
	{
		//left leaf case
		root->leftChild = (struct treeNode *) calloc((size_t) 1, sizeof(struct treeNode));
		root->leftChild->name = entryList->left;
		struct oneLine *rightEntryList = globalEntryList[labelList->right];
		struct nodeLabel *rightLabelList = globalLabelList[labelList->right];
		root->rightChild = makeTree(rightEntryList, rightLabelList, globalEntryList, globalLabelList);
	}
	else if(labelList->right == -1 && labelList->left != -1)
	{
		//right leaf case
		root->rightChild = (struct treeNode *) calloc((size_t) 1, sizeof(struct treeNode));
		root->rightChild->name = entryList->right;
		struct oneLine *leftEntryList = globalEntryList[labelList->left];
		struct nodeLabel *leftLabelList = globalLabelList[labelList->left];
		root->leftChild = makeTree(leftEntryList, leftLabelList, globalEntryList, globalLabelList);
	}
	else
	{

		struct oneLine *rightEntryList = globalEntryList[labelList->right];
		struct nodeLabel *rightLabelList = globalLabelList[labelList->right];
		root->rightChild = makeTree(rightEntryList, rightLabelList, globalEntryList, globalLabelList);

		struct oneLine *leftEntryList = globalEntryList[labelList->left];
		struct nodeLabel *leftLabelList = globalLabelList[labelList->left];
		root->leftChild = makeTree(leftEntryList, leftLabelList, globalEntryList, globalLabelList);
	}


	return root;
}

void print_tree(struct treeNode *root) {
	if (root->leftChild == NULL && root->rightChild == NULL)
	{
		printf("leaf (%s)\n", root->name);
		return;
	}
	printf("interior (%s) start\n", root->name);
	printf("interior (%s) left recursive call coming up\n", root->name);
	print_tree(root->leftChild);
	printf("interior (%s) right recursive call coming up\n", root->name);
	print_tree(root->rightChild);
}

int calculateTree(struct treeNode *root)
{
	int result;
	printf("%s\n", root->name);
	printf("strcmp to *: %d\n", strcmp(root->name, "*"));
	printf("strcmp to +: %d\n", strcmp(root->name, "+"));
	if (root->leftChild == NULL && root->rightChild == NULL)
	{
		result = atoi(root->name);
		return result;
	}
	else if(root->name[0] == '*')
	{
		printf("got here *\n");
		return (calculateTree(root->leftChild) * calculateTree(root->rightChild));
	}
	//root->name = + case
	else
	{
		printf("got here +\n");
		return (calculateTree(root->leftChild) + calculateTree(root->rightChild));
	}
	
}

int main()
{
	FILE *fp = fopen(FILE_NAME, "r");					//Open tree file

	char **entryList = (char **) calloc((size_t) 1000, sizeof(char*));	//allocate memory for entryList
	struct treeArr *nodeArray = (struct treeArr *) calloc((size_t) 1, sizeof(struct treeArr));
	struct treeStructsArr *inputArray = (struct treeStructsArr *) calloc((size_t) 1, sizeof(struct treeStructsArr));
	struct treeNode *root = (struct treeNode *) calloc((size_t) 1, sizeof(struct treeNode));


//printf("About to makeTreeArr\n");
	nodeArray = makeTreeArr(fp);						//entryList can hold 999 characters at most, plus the null terminator
//printf("Got past makeTreeArr\n");
	inputArray = makeStructs(nodeArray);
//printf("Got past makeStructs\n");

	// separate out the separate structs and pass them in
	// entryLine, labelLine, globalEntryList, globalLabelList
	struct oneLine **globalEntryList = inputArray->array;
	struct nodeLabel **globalLabelList = inputArray->format;

	struct oneLine *firstLine  = NULL;
	if (globalEntryList != NULL)
	{
		firstLine = globalEntryList[0];
	}
	struct nodeLabel *firstLabel = NULL;
	if (globalLabelList != NULL)
	{
		firstLabel = globalLabelList[0];
	}
	entryList = nodeArray->array;

//printf("\n");
	//use entryList to make tree
	root = makeTree(firstLine, firstLabel, globalEntryList, globalLabelList);
	print_tree(root);
//printf("Got past makeTree\n");
	int result = calculateTree(root);
	printf("result: %d\n", result);

	return 0;
}
