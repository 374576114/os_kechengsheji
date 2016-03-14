#include "task.h"

float mem_f()
{
	FILE *fp = NULL;
	const char *file = "/proc/meminfo";
	int total, free, buffers, cached;
	float mem;

	fp = fopen(file, "r");
	if(fp == NULL){
		printf("file can not open\n");
		exit(1);
	}
	fscanf(fp, "%*s %d %*s %*s %d %*s %*s %d %*s %*s %d", &total, &free, &buffers, &cached);
	
	mem = 1.0 * 100 *(total -free - buffers - cached) / total;

	fclose(fp);
	return mem;
}

int main(){
	printf("%.3f \n", mem_f());
	return 0;
}
