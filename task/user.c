#include "task.h"

int  user_find_f(int id, char *name_)
{
	FILE *fp = NULL;
	char name[128];
	char buf[512];
	char ch;
	int n;
	int i;

	fp = fopen("/etc/passwd", "r");
	if(fp == NULL){
		printf("/etc/passwd can not open\n");
		return -1;
	}

	while(!feof(fp)){
		for(i=0; i<127; i++){
			if( (ch=fgetc(fp)) != ':'){
				name[i] = ch;
			}else{
			  break;
		  }
		}
		name[i] = 0;
		fscanf(fp, "%*c%*c%d", &n);
		if( n == id){
			strcpy(name_ ,name);
			//printf("%s\n", name);
			return 0;
		}else{
			fgets(buf, 500, fp);
		}
	}
	strcpy(name_, "--");
	return -1;
}

int main()
{
	int n;
	char name[100];
	printf("please input the num\n:");
	scanf("%d", &n);
	user_find_f(n, name);
	printf("%s\n", name);

	return 0;
}


