#include "task.h"

int version_f(char *ver)
{
	FILE *fp = NULL;
	
	fp = fopen("/proc/version", "r");
	if(fp == NULL){
		printf("can not open the version file\n");
		strcpy(ver, "--");
		return -1;
	}

	fgets(ver, 32, fp);
	fclose(fp);

	return 0;
}

int main(){
	char ver[50];
	version_f(ver);
	printf("%s\n", ver);
	return 0;
}
	

