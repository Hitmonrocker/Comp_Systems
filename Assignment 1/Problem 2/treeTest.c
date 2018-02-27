#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char** copyStringArray(char** original, int numChildrenNames);
char **addChild (char currentChar, char **childrenNames);

struct tree_node
{
  struct tree_node **children;											//** = array
  char *name;
  int numChildren;
};

struct processedLine
{
  char *name;
  int numChildren;
  char **childrenNames;
};
////////////////////////////////////////////////////////////////////////

struct processedLine **processLines (char *line, int numLines)
{
	//initialization
	//counters
	int i = 0;
	int resultsIndex = 0;
	int nodeCount = 0;
	//function return value
	struct processedLine **results = (struct processedLine **) calloc ((size_t) numLines, sizeof (struct processedLine *));
	//booleans
	int isNumStr = 0;		//0 = FALSE
	int isFirstNode = 0;
	int isProcessedNodeCount = 0;
	//arrays
	int numStrBounds[2] = { 0, 0 };
	char savedNodeName[2] = { '\0', '\0' };

	//oneResult is the processed line of a single line as written in the original file
	struct processedLine *oneResult = (struct processedLine *) calloc ((size_t) 1, sizeof (struct processedLine));
	char mBuf[1] = { '\0' };
	oneResult->name = mBuf;
	oneResult->numChildren = 0;
	oneResult->childrenNames = (char **) calloc ((size_t) 50, sizeof (char *));
	
	//for loop to process entire tree from input
    for (i = 0; i < (int) strlen (line); i++)
    {
      char currentChar = line[i];


      //edge cases
      //first letter
      if (i == 0)
	  {
	  isFirstNode = 1;
	  char buf[2] = { currentChar, '\0' };
	  savedNodeName[0] = currentChar;
	  oneResult->name = buf;
	  continue;
	  }
	  
	  //last character (zero)
      if (i == strlen (line) - 1)
	  {
	  //process last zero
	  oneResult->name = (char *) calloc ((size_t) 2, sizeof (char));
	  oneResult->name[0] = savedNodeName[0];
	  oneResult->numChildren = 0;
	  results[resultsIndex] = oneResult;
	  break;
	  }

	  //number case
      if (currentChar >= '0' && currentChar <= '9')
	  {
	  //zero case
	  if (isNumStr == 0)
	  {
	      isNumStr = 1;
	      numStrBounds[0] = i;
	      numStrBounds[1] = i;
	      if (isFirstNode == 1)
	      {
		  isFirstNode = 0;
		  continue;
	      }
	      results[resultsIndex] = (struct processedLine *) calloc ((size_t) 1, sizeof (struct processedLine));
	      results[resultsIndex]->name = (char *) calloc ((size_t) 2, sizeof (char));
	      results[resultsIndex]->name[0] = oneResult->name[0];	// strcpy
	      results[resultsIndex]->numChildren = oneResult->numChildren;

		  results[resultsIndex]->childrenNames = copyStringArray(oneResult->childrenNames, oneResult->numChildren);

	      resultsIndex += 1;
	      char resetBuf[2] = { '\0', '\0' };
	      oneResult->name = resetBuf;
	      oneResult->numChildren = 0;
	      oneResult->childrenNames = (char **) calloc ((size_t) 50, sizeof (char *));


	      oneResult->name[0] = savedNodeName[0];
	      savedNodeName[0] = '\0';	//reset
	      char OneResultName[2] = { '\0', '\0' };

	  }
	  else
	  {
	      numStrBounds[1] = i;
	  }

	}
    else
	{
	  if (isNumStr == 0)
	  {
	      //Child node logic goes here
	      oneResult->childrenNames = addChild (currentChar, oneResult->childrenNames);
	      nodeCount--;

	      if (nodeCount == 0)	//Processed all children
          {
		  savedNodeName[0] = currentChar;
	      }

	  }
	  else
	  {
	      //End number case
	      char *cpyBuf = (char *) calloc ((size_t) (numStrBounds[1] - numStrBounds[0] + 1), sizeof (char));
	      char *fakeNewLine = line + numStrBounds[0];
	      char *substring = strncpy (cpyBuf, fakeNewLine, (size_t) (numStrBounds[1] - numStrBounds[0] + 1));

	      nodeCount = atoi (substring);
	      numStrBounds[0] = 0;
	      numStrBounds[1] = 0;
	      isNumStr = 0;
	      isProcessedNodeCount = 1;
	      //Current char is a child node name, so we must add it to struct
	      oneResult->numChildren = nodeCount;
	      if (nodeCount > 0)
		  {
		  oneResult->childrenNames = addChild (currentChar, oneResult->childrenNames);
		  }
	      else
		  {
		  results[resultsIndex] = (struct processedLine *) calloc ((size_t) 1, sizeof (struct processedLine));
		  results[resultsIndex]->name = (char *) calloc ((size_t) 2, sizeof (char));
		  results[resultsIndex]->name[0] = oneResult->name[0];
		  results[resultsIndex]->numChildren = oneResult->numChildren;
		  results[resultsIndex]->childrenNames = copyStringArray(oneResult->childrenNames, oneResult->numChildren);
		  resultsIndex += 1;
		  savedNodeName[0] = currentChar;
          }
	  }

	}


	}


	return results;
}

char **addChild(char currentChar, char **childrenNames)
{
	char *buffer = (char*) calloc((size_t)2, sizeof(char));
	buffer[0] = currentChar;
	char *currentChildName = childrenNames[0];
	int lastIndex = 0;

	while(currentChildName != NULL)
	{
		lastIndex++;
    currentChildName = childrenNames[lastIndex];
	}

	childrenNames[lastIndex] = buffer;
	return childrenNames;
}

char** copyStringArray(char** original, int numChildrenNames)
{
	char** results = (char**) malloc(sizeof(char*)*numChildrenNames);
	int i, j;
	for(i = 0; i < (int) sizeof(char*)*numChildrenNames; i++)
	{
		char** dangerPointer = results + i;
		*dangerPointer = NULL;
		//end calloc initialization
	}
	for(j = 0; j < numChildrenNames; j++)
	{
		results[j] = (char*) calloc(((size_t)2), sizeof(char));
		results[j][0] = original[j][0];
	}
	return results;
}

/*
int main()
{
	struct processedLine **result = processLines("A2BCB1DD0C0", 4);

	int i;

	for(i = 0; i<4; i++)
	{
		struct processedLine *derp = result[i];
		printf("%s\n%d\n", derp->name, derp->numChildren);
	}

	return 0;
}
*/

