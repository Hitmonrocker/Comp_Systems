#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char** copyStringArray(char** original, int numChildrenNames);
char** addChild (char currentChar, char **childrenNames);

struct tree_node
{
	struct tree_node **children;										//** = array (actually a pointer to a pointer)
																		//children is an array of tree nodes
	char *name;															//char * is a string
	int numChildren;
};

struct processedLine
{
	char *name;
	int numChildren;
	char **childrenNames;												//childrenNames is an array of strings
};
////////////////////////////////////////////////////////////////////////
/*this function takes a line from the input text file and converts it to
 * an array.
*/
struct processedLine **processLines (char *line, int numLines)
{
	//initialization
	
	//counters
	int i = 0;
	int resultsIndex = 0;
	int nodeCount = 0;													//nodeCount is a counter for the current line's number of children
	
	//function return value (need to allocate memory for this, so calloc here)
	struct processedLine **results = (struct processedLine **)\
		calloc ((size_t) numLines, sizeof (struct processedLine *));
	
	//booleans
	int isNumStr = 0;													//0 == FALSE
	int isFirstNode = 0;
	int isProcessedNodeCount = 0;
	
	//arrays
	int numStrBounds[2] = {0, 0};										//array to determine start and end indices of multidigit number
	char savedNodeName[2] = {0, 0};										//array to save node name of first node in the input file line

	//oneResult is a pointer to a struct that represents a single line
	//	in the original file
	struct processedLine *oneResult = (struct processedLine *)\
		calloc ((size_t) 1, sizeof (struct processedLine));
	char mBuf[1] = {'\0'};												//mBuf = memory buffer
	
	//initialize values of temporary processed line pointer
	oneResult->name = mBuf;
	oneResult->numChildren = 0;
	oneResult->childrenNames = (char **) calloc ((size_t) 50, sizeof (char *));		//allocate space for string array
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//for loop to process entire tree from input
    for (i = 0; i < (int) strlen (line); i++)
    {
		char currentChar = line[i];
	    //edge cases
	    //first letter
	    if (i == 0)
		{
			isFirstNode = 1;											//isFirstNode = TRUE
			char buf[2] = {currentChar, '\0'};							//buffer for name (assuming name is just a letter)
			savedNodeName[0] = currentChar;								//save the current node's name in string savedNodeName
			oneResult->name = buf;
			
			continue;
		}
		  
		//last relevant character in file (zero)
	    if (i == strlen (line) - 1)
		{
			//process last zero
			oneResult->name = (char *) calloc ((size_t) 2, sizeof (char));	//allocate memory for name string
			oneResult->name[0] = savedNodeName[0];							//resulting current node's name is set to the node
																		//	name saved previously
			oneResult->numChildren = 0;
			results[resultsIndex] = oneResult;
			break;
		}
	
		//number case
	    if (currentChar >= '0' && currentChar <= '9')
		{
			//letter (or symbol) to number transition
			if (isNumStr == 0)											//if we were previously reading characters that were NOT numbers
			{
			    isNumStr = 1;											//this is a number string, so set isNumStr = TRUE
			    numStrBounds[0] = i;									//set leftbound and rightbount to index of current char
			    numStrBounds[1] = i;
			    if (isFirstNode == 1)
			    {
					isFirstNode = 0;									//program has gotten past the first node in the line, so set
																		//	isFirstNode = FALSE
					continue;											//go to next iteration of whole for loop
				}
				results[resultsIndex] = (struct processedLine *)\
					calloc ((size_t) 1, sizeof (struct processedLine));	//allocate memory for current tree node's place in the results array
				results[resultsIndex]->name =\
					(char *) calloc ((size_t) 2, sizeof (char));		//allocate memory for the tree node's name variable
					
				//set tree node variables in results array
				if(currentChar == '0')
				{
					results[resultsIndex]->name[0] = oneResult->name[0];	
				}
				else
				{
					results[resultsIndex]->name[0] = savedNodeName[0];
				}
				
				char resetBuf[2] = { '\0', '\0' };						//reset buffer for next iteration
				
				//reset temporary processedLine pointer's variables
				oneResult->name = resetBuf;
				oneResult->numChildren = 0;
				oneResult->childrenNames = (char **) calloc ((size_t) 50, sizeof (char *));
	
			    oneResult->name[0] = savedNodeName[0];
			    savedNodeName[0] = '\0';	//reset
			    //char OneResultName[2] = { '\0', '\0' };

			}	//end !number to number case
			//number to !number case
			else
			{
				//if program gets here, then number continues
				numStrBounds[1] = i;
			}
		}
		//if the program gets here, then the current character is NOT a number
	    else
		{
			//case: previous character was also not a number
			if (isNumStr == 0)
			{
			    //Child node logic goes here
			    nodeCount--;											//decrement child node counter for current line
		
				if (nodeCount == 0)	//Processed all children
				{					
					results[resultsIndex] = (struct processedLine *)\
						calloc ((size_t) 1, sizeof (struct processedLine));
						
					results[resultsIndex]->name = (char *)\
						calloc ((size_t) 2, sizeof (char));
					
					
					results[resultsIndex]->name[0] = oneResult->name[0];
					results[resultsIndex]->numChildren = oneResult->numChildren;
					
					results[resultsIndex]->childrenNames =\
						copyStringArray(oneResult->childrenNames, oneResult->numChildren);
						
					resultsIndex += 1;									//go to next index in results array
					
					savedNodeName[0] = currentChar;						//now on the next line, so the current character is the
																		//	name of the node we are looking at as the parent of
																		//	the children on the line
					oneResult->name[0] = savedNodeName[0];
			    }
			    
			    oneResult->childrenNames = addChild\
					(currentChar, oneResult->childrenNames);			//this is one of the child nodes of the current node, so
																		//	add the child's name to the current node's array of
																		//	child names
		
			}
			//case: previous character was a number
			else
			{				
				char *cpyBuf = (char *) calloc ((size_t)\
					(numStrBounds[1] - numStrBounds[0] + 1), sizeof (char));	//buffer to hold string
					
			    char *fakeNewLine = line + numStrBounds[0];						//???
			    
			    char *substring = strncpy (cpyBuf, fakeNewLine, (size_t)\
					(numStrBounds[1] - numStrBounds[0] + 1));					//actual substring of number to put in buffer
		
				nodeCount = atoi (substring);							//convert substring of number to integer
				
				//reset string bounds of number input, number string boolean and processed node count boolean
			    numStrBounds[0] = 0;
			    numStrBounds[1] = 0;
				isNumStr = 0;
			    isProcessedNodeCount = 1;
			    
			    //Current char is a child node name, so we must add it to struct
				oneResult->numChildren = nodeCount;						//set the number that was read in as the current node's count of child nodes
				/*
				 * Now nodeCount can be used as a counter to indicate how many
				 * of the next elements in the array need to be read in
				 * as child nodes of the current node
				*/
			    if (nodeCount > 0)
				{
					oneResult->childrenNames = addChild\
						(currentChar, oneResult->childrenNames);		//as long as nodeCount does not reach 0, the current array
				}														//	element is a child node, so add it to the first node's
																		//	list of children
				/*
				 * if get here, then ready to store the first node of the
				 * line and its members in the results array.
				*/
			    else
				{
					results[resultsIndex] = (struct processedLine *)\
						calloc ((size_t) 1, sizeof (struct processedLine));
						
					results[resultsIndex]->name = (char *)\
						calloc ((size_t) 2, sizeof (char));
					
					
					results[resultsIndex]->name[0] = oneResult->name[0];
					results[resultsIndex]->numChildren = oneResult->numChildren;
					
					results[resultsIndex]->childrenNames =\
						copyStringArray(oneResult->childrenNames, oneResult->numChildren);
						
					resultsIndex += 1;									//go to next index in results array
					savedNodeName[0] = currentChar;						//recall that we are at the first character of the next line,
																		//	so store the current character as the current node that we
																		//	are looking at
		        }
			}
		}
	}
	return results;
}

char **addChild(char currentChar, char **childrenNames)
{
	char *buffer = (char*) calloc((size_t)2, sizeof(char));				//allocate memory
	buffer[0] = currentChar;											//put current character in the first index of buffer
	char *currentChildName = childrenNames[0];							//declare a pointer to go through the childrenNames array
	int lastIndex = 0;													//lastIndex is a counter used for going through all the
																		//	indices of the array
	while(currentChildName != NULL)
	{
		lastIndex++;
		currentChildName = childrenNames[lastIndex];
	}

	childrenNames[lastIndex] = buffer;									//end childrenNames array with buffer as last element?
	return childrenNames;
}

char** copyStringArray(char** original, int numChildrenNames)
{
	char** results = (char**) malloc(sizeof(char*)*numChildrenNames);	//allocate memory for resulting copied string array
	int i, j;															//i and j are counters
	
	for(i = 0; i < (int) sizeof(char*)*numChildrenNames; i++) 			//loop through all children names
	{
		char** dangerPointer = results + i;
		*dangerPointer = NULL;
		//end calloc initialization
	}
	
	for(j = 0; j < numChildrenNames; j++)								//loop through all children names
	{
		results[j] = (char*) calloc(((size_t)2), sizeof(char));			//allocate memory for array entry
		results[j][0] = original[j][0];									//copy original string to results string
	}
	return results;
}

/*
int main()
{
	struct processedLine **result = processLines("A2BCB2DED0E0C0", 5);

	int i;

	for(i = 0; i<5; i++)
	{
		struct processedLine *derp = result[i];
		printf("%s\t%d\n", derp->name, derp->numChildren);
	}

	return 0;
}
*/

