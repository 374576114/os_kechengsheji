#include "task.h"
/*
 * 将 用户组-ID 填入链表 u_name_to_id_t
 */
int  user_find_list(u_name_to_id_t **head)
{
	FILE *fp = NULL;
	u_name_to_id_t *p;
	char name[128];
	char ch;
	int i;

	(*head) = (u_name_to_id_t*)malloc(sizeof(struct user_name_to_id));
	(*head)->next = NULL;
	(*head)->id = -1;
	p = *head;

	fp = fopen("/etc/passwd", "r");
	if(fp == NULL){
		printf("/etc/passwd can not open\n");
		perror("passwd");
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
		strcpy(p->name, name);
		fscanf(fp, "%*c%*c%d", &(p->id));
		//printf("%s \n", p->name);
		while(!feof(fp) && (ch=fgetc(fp))!='\n'); 
		//printf("%d xxxx\n", feof(fp));
		p->next = (u_name_to_id_t*)malloc(sizeof(struct user_name_to_id));
		p = p->next;
		p->id = -1;
		strcpy(p->name, "--");
		p->next = NULL;
	}
	fclose(fp);
	return 0;
}
/*
 * 测试
 */
void print(u_name_to_id_t *head)
{
	u_name_to_id_t *p = head;
	while(p){
		printf("%s %d\n", p->name, p->id);
		p = p->next;
	}
}

/*
 * 查找对应用户组  by ID
 */
int user_find_f(int id, char *name, u_name_to_id_t *head)
{
	u_name_to_id_t *p = head;
	while(p){
		if(p->id == id){
			strcpy(name, p->name);
			//printf("%s \n", name);
			return 0;
		}
		p = p->next;
	}
	strcpy(name, "--");
	return -1;
}

/*int main(){
	char name[20];
	u_name_to_id_t *head = NULL;
	user_find_list(&head);
	user_find_f(000,name, head); 
	//print(head);
}*/

/*int main()
{
	int n;
	char name[100];
	printf("please input the num\n:");
	scanf("%d", &n);
	user_find_f(n, name);
	printf("%s\n", name);

	return 0;
}
*/

