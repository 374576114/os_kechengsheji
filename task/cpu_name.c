#include "task.h"

int read_cpu_info(char *name)
{
	FILE *fp = NULL;
	char buf[512]; 

	fp = fopen("/proc/cpuinfo", "r");
	if(fp == NULL){
		printf("can not open /proc/cpuinfo\n");
		strcpy(buf, "--");
		return -1;
	}

	fgets(buf, 512, fp);
	fgets(buf, 512, fp);
	fgets(buf, 512, fp);
	fgets(buf, 512, fp);
	fgets(buf, 512, fp);

	sprintf(name, "%s", buf+13); 
	return 0;
}
/*

int main()
{
	char name[50];
	read_cpu_info(name);
	printf("%s", name);
	return 0;
}*/



