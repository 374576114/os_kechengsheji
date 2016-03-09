#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE *fp = NULL;
	char name[5] = "";
	float f, a, idle;
	int t1[9] = {0};
	int t2[9] = {0};
	int a1, a2;
	int i, j;

	fp = fopen("/proc/stat", "r");
	if(fp == NULL){
		printf("Can not open the file\n");
		exit(1);
	}
	while(1){
		//fseek(fp, 0, SEEK_SET);
		
		fp = fopen("/proc/stat", "r");
		fscanf(fp, "%s %d %d %d %d %d %d %d %d %d",name, t1+0,t1+1,t1+2,t1+3,t1+4,t1+5,t1+6, t1+7, t1+8);
		fclose(fp);
		sleep(1);
		//fseek(fp, 0, SEEK_SET);
		fp = fopen("/proc/stat", "r");
		fscanf(fp, "%s %d %d %d %d %d %d %d %d %d",name, t2+0,t2+1,t2+2,t2+3,t2+4,t2+5,t2+6, t2+7, t2+8);
		fclose(fp);

		a1 = 0;
		a2 = 0;

		for(i=0; i<9; i++){
			a1 += t1[i];
		}
		i = 0;
		for(i=0; i<9; i++){
			a2 += t2[i];
		}
		printf("a1 %d a2 %d \n", a1, a2);
		a = (float)(a2-a1);
		idle =(float)(t2[3] - t1[3]);
		f = (a - idle) /a *100;
		printf("%.3f %.3f cpu : %.3f%%\n", a, idle, f);
	}
	return 0;
}

	


	

