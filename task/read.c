#include "task.h"

int main()
{
	FILE *fp = NULL;
	char name[50];
	char ch;
	int i;

	fp = fopen("./test.txt", "r");
	
	while(fgetc(fp) != '(' && !feof(fp));

	for(i=0; i<50; i++){
		if((ch=fgetc(fp)) != ')')
		  name[i] = ch;
		else
		  break;
	}
	name[i] = 0;

	printf("%s\n", name);
	
	return 0;
}
