#include "task.h"
/*
 * 功能：取时间间隔为1s的CPU利用率
 * 返回值： cpu利用率百分比
 */
float cpu_f(){
	FILE *fp = NULL;
	char name[5] = "";
	float f, a, idle;
	int t1[9] = {0};
	int t2[9] = {0};
	int a1=0, a2=0;
	int i;

	fp = fopen("/proc/stat", "r");
	if(fp == NULL){
		printf("Can not open the file\n");
		exit(1);
	}

	fscanf(fp, "%s %d %d %d %d %d %d %d %d %d",name, t1+0,t1+1,t1+2,t1+3,t1+4,t1+5,t1+6, t1+7, t1+8);
	fclose(fp);

	sleep(1);
	
	fp = fopen("/proc/stat", "r");
	if(fp == NULL){
		printf("Can not open the file\n");
		exit(1);
	}
	fscanf(fp, "%s %d %d %d %d %d %d %d %d %d",name, t2+0,t2+1,t2+2,t2+3,t2+4,t2+5,t2+6, t2+7, t2+8);
	fclose(fp);


	for(i=0; i<9; i++){
		a1 += t1[i];
	}
	for(i=0; i<9; i++){
		a2 += t2[i];
	}

	a = (float)(a2-a1);
	idle =(float)(t2[3] - t1[3]);
	f = (a - idle) /a *100;
	return f;
}

int main()
{
	printf("%.3f\n", cpu_f());
	return 0;
}
