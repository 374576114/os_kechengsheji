/*
 * 功能:完成文件复制
 * 题目 1.1
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){
	FILE *fp = NULL;
	FILE *fpw = NULL;
	char buff[200] = "";
	char name[200] = "";
	int n = 0;

	usleep(1000000);
	printf("please input the file path and name you want to copy\n");
	scanf("%s", name);

	//fp = fopen("./tmp.txt", "rw");
	fp = fopen(name, "r");
	fpw = fopen("./copy.txt", "w");
	
	if(fp == NULL){
		printf("ERROR! File can not open\n");
		exit(1);
	}

	while(! feof(fp)){
		n = fread(buff, sizeof(char), 200, fp);
		fwrite(buff, sizeof(char), n, fpw);
	}

	printf("Copy end, the file name is \"copy.txt\"\n");
	return 0;
}
		
