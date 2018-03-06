#include <stdio.h>
#include <stdlib.h>

#define FILE_NAME "text.txt"

int main()
{
	FILE* fp = fopen(FILE_NAME, "w");
	int i, num;
	
	for(i = 0; i < 100; i++)
	{
		num = rand() % 100;
		fprintf(fp, "%d\n", num);
	}
	
	fclose(fp);
	
	return 0;
}
