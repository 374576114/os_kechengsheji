#include "task.h"

int hostname(char *name)
{
	FILE *fp = NULL;

	fp = fopen("/proc/sys/kernel/hostname", "r");
	if(fp == NULL){
		printf("cannot open the hostname\n");
		strcpy(name, "--");
		return -1;
	}

	fscanf(fp, "%s", name);
	return 0;
}

int main()
{
	char name[100];
	hostname(name);
	printf("%s\n", name);
	return 0;
}

