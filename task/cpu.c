#include "task.h"
/*
 * 功能：取时间间隔为1s的CPU利用率
 * 返回值： cpu利用率百分比
 */
float cpu_f(int  *pall, int  *pidle, int n){
	FILE *fp = NULL;
	char name[5] = "";
	float f, a, idle;
	int a1 = 0;
	int t1[9] = {0};
	int i;

	fp = fopen("/proc/stat", "r");
	if(fp == NULL){
		printf("Can not open the file\n");
		exit(1);
	}

	fscanf(fp, "%s %d %d %d %d %d %d %d %d %d",name, t1+0,t1+1,t1+2,t1+3,t1+4,t1+5,t1+6, t1+7, t1+8);
	fclose(fp);

	for(i=0; i<9; i++){
		a1 += t1[i];
	}

	if(n == 1){
		*pall = a1;
		*pidle = t1[3];
		return 0;
	} else {
		//printf("%d %d\n", *pall, *pidle);
		a = (float) (a1-*pall);
		idle = (float)(t1[3] - *pidle);
		f = (a-idle)/a * 100;
	}
	return f;
}
/*
int main()
{
	int a, b;
	float c;
	cpu_f(&a, &b, 1);
	printf("%d %d\n", a,b);
	sleep(1);
	c = cpu_f(&a, &b, 2);
	printf("%.2f\n", c);

	return 0;
}
*/
