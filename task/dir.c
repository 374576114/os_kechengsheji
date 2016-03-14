#include "task.h"

typedef struct p_info{
	int id;
	int flag;
	char name[100];
	char mem[50];
	int user;
	long cpu_total1;
	long cpu_total2;
	long process1;
	long process2;
	float cpu;
	int priority;
	int nice;
	struct p_info *p;
	struct p_info *next;
}p_info_t;
/*
 * 传入ID号dir
 */
int get_process_info(const char *dir, struct p_info *p, int n)
{
	FILE *fcpu, *fmem, *fp_cpu;
	int t_cpu[10], t_p[4];
	int cpu_total=0, pcpu_total=0 , i;
	char ch , buf[512];
	int mem_count;
	float f_mem, a, b;

	if(chdir(dir) == -1){/*该进程已经不存在*/
		p->flag = 0;
		return -1;
	}

	fcpu = fopen("/proc/stat", "r");
	if(fcpu == NULL){
		printf("can not open /proc/stat\n");
		p->flag = 0;
		return -1;
	}

	fmem = fopen("./status", "r");
	if(fmem == NULL){
		printf("can not open status\n");
		p->flag = 0;
		return -1;
	}

	fp_cpu = fopen("./stat", "r");
	if(fp_cpu == NULL){
		printf("can not open stat\n");
		p->flag = 0;
		return -1;
	}
	/*cpu数据统计*/
	fscanf(fcpu, "%*s %d %d %d %d %d %d %d %d %d %d", t_cpu,t_cpu+1, t_cpu+2, t_cpu+3, t_cpu+4, t_cpu+5,t_cpu+6,t_cpu+7, t_cpu+8,t_cpu+9);
	for(i=0; i<10; i++){
		cpu_total += t_cpu[i];
	}
	if(n == 1)
	  p->cpu_total1 = cpu_total;
	else
	  p->cpu_total2 = cpu_total;
	fclose(fcpu);

	/*process 数据统计*/
	fscanf(fp_cpu, "%d", &(p->id));
	while(fgetc(fp_cpu)!='(' && !feof(fp_cpu));
	for(i=0; i<50; i++){
		if((ch = fgetc(fp_cpu)) != ')'){
			(p->name)[i] = ch;
		}else{
			break;
		}
	}
	(p->name)[i] = 0;
	fscanf(fp_cpu,"%*s %*d %*d %*d %*d %*d %d", &mem_count);
	fscanf(fp_cpu,"%*d %*d %*d %*d %d %d %d %d", t_p, t_p+1, t_p+2, t_p+3);
	pcpu_total = t_p[0] + t_p[1] + t_p[2] + t_p[3];
	if(n == 1){
		p->process1 = pcpu_total;
	}else{
		p->process2 = pcpu_total;
	}
	fscanf(fp_cpu, "%d %d", &(p->priority), &(p->nice));
	fclose(fp_cpu);

	/*用户组*/
	for(i=0; i<8; i++){
		fgets(buf, 511, fmem);
	}
	fscanf(fmem,  "%*s %d", &(p->user));
	for(i=0; i<8; i++){
		fgets(buf, 511, fmem);
	}
	fscanf(fmem, "%s %d", buf, &mem_count);
	if(strcmp(buf, "VmRSS:") !=0){
	  mem_count = -1;
	  sprintf(p->mem, "不适用");
	}else {
		f_mem = 1.0 * mem_count / 1024;
		sprintf(p->mem, "%.1f MB", f_mem);
	}
	fclose(fmem);

	p->flag ++;

	/*统计*/
	if(n == 2){
		a = (float) (p->process2 - p->process1);
		b = (float) (p->cpu_total2 - p->cpu_total1);
		p->cpu = a/b*100;
		//printf("%.2f\n", p->cpu);
	}

	if(chdir("..") == -1){/*该进程已经不存在*/
		chdir("/proc/");
		p->flag = 0;
	}
	//printf("%d\n", p->id);
	return 0;
}
/*
 * 按照名字排序
 */
p_info_t * sort_by_name(p_info_t *phead)
{
	p_info_t *p = phead, *pmax_head=NULL;
	p_info_t *p1, *p2;
	char name[100] = "z";
	
	while(p){
		if(strcmp(name, p->name) >=0 ){
			strcpy(name, p->name);
			pmax_head = p;
		}
		p = p->next;
	}

	pmax_head->cpu_total1 = 0;
	p1 = pmax_head;
	p2 = p1;

	while(1){
		p = phead;
		while(p!=NULL && p->cpu_total1 == 0)
		  p = p->next;
		if(!p)
		  break;
		//cpu = p->cpu;
		strcpy(name, p->name);
		p1 = p;

		while(p){
			if(p->cpu_total1 !=0 && strcmp(name, p->name)>=0){
				//cpu = p->cpu;
				strcpy(name ,p->name);
				p1 = p;
			}
			p = p->next;
		}
		p1->cpu_total1= 0;
		p2->p = p1;
		p2 = p1;
	}
	p1->p = NULL;
	return pmax_head;
}

/*
 * 按照cpu利用率排序
 */
p_info_t * sort_by_cpu(p_info_t *phead)
{
	p_info_t *p = phead, *pmax_head=NULL;
	p_info_t *p1, *p2;
	float cpu = -1.0;
	
	while(p){
		if(p->cpu > cpu){
			cpu = p->cpu;
			pmax_head = p;
		}
		p = p->next;
	}

	pmax_head->process1 = 0;
	p1 = pmax_head;
	p2 = p1;

	while(1){
		p = phead;
		while(p!=NULL && p->process1 == 0)
		  p = p->next;
		if(!p)
		  break;
		cpu = p->cpu;
		p1 = p;

		while(p){
			if(p->process1!=0 && p->cpu > cpu){
				cpu = p->cpu;
				p1 = p;
			}
			p = p->next;
		}
		p1->process1 = 0;
		p2->p = p1;
		p2 = p1;
	}
	p1->p = NULL;
	return pmax_head;
}

/*
 * 按照mem使用情况排序
 */
p_info_t * sort_by_mem(p_info_t *phead)
{
	p_info_t *p = phead, *pmax_head=NULL;
	p_info_t *p1, *p2;
	float mem = -1.0;
	
	while(p){
		if(atof(p->mem) > mem){
			mem = atof(p->mem);
			pmax_head = p;
		}
		p = p->next;
	}

	pmax_head->process1 = 0;
	p1 = pmax_head;
	p2 = p1;

	while(1){
		p = phead;
		while(p!=NULL && p->process1 == 0)
		  p = p->next;
		if(!p)
		  break;
		mem = atof(p->mem);
		p1 = p;

		while(p){
			if(p->process1!=0 && atof(p->mem) > mem){
				mem = atof(p->mem);
				p1 = p;
			}
			p = p->next;
		}
		p1->process1 = 0;
		p2->p = p1;
		p2 = p1;
	}
	p1->p = NULL;
	return pmax_head;
}

void print_cpu_sort(p_info_t *head)
{
	p_info_t *p = head;
	while(p){
		printf("%s %d %s %lu\n", p->name, p->id, p->mem, p->process1);
		p = p->p;
	}
}
int printall(p_info_t *p)
{
	struct p_info s;
	p_info_t * p1 = NULL;
	//p1 = sort_by_cpu(p);
	//p1 = sort_by_mem(p);
	p1 = sort_by_name(p);
	//while(p->next){
		//if(p->cpu > 1.0){
		//s = *p;
		s = *p1;
		printf("flag :%d", s.flag);
		printf("name: %s\n", s.name);
		printf("%d\n", s.id);
		printf("use:%ld\n", s.process2 - s.process1);
		printf("all:%ld\n", s.cpu_total2 - s.cpu_total1);
		printf("%d\n", s.user);
		printf("%s\n", s.mem);
		printf("%d\n", s.priority);
		printf("%d\n", s.nice);
		printf("cpu: %.2f\n", s.cpu);
		getchar();
		print_cpu_sort(p1);
		//print_mem_sort(p1);
		//}
		//p = p->next;
	//}
	return 0;
}


int complete_link_list(p_info_t *phead, const char *dir, int id, int n)
{
	p_info_t *p = phead;
	p_info_t *p1;

	/*寻找是否已经存在*/
	if(p->id == -1){ /*表示链表未被初始化*/
		p->flag = 0;
		get_process_info(dir, p, n);
		return 0;
	}

	/*遍历寻找*/
	while(p->next){
		if(p->id == id){/*相等找到了*/
			get_process_info(dir, p, n);
			return 0;
		}

		if(p->id > id){/*需要在前面添加*/
			p1 = (p_info_t*)malloc(sizeof(struct p_info));
			if(p1 == NULL){
				printf("memory error\n");
				exit(1);
			}
			p1->flag = 0;
			//p1->pre = p->pre;
			p1->next = p;
			//p->pre = p1;
			//p->pre->next = p1;
			p = p1;
			get_process_info(dir, p, n);
			return 0;
		}
		p = p->next;
	}

	if(p->id == id){/*需要在结尾处处理*/
		get_process_info(dir, p, n);
		return 0;
	}else{
		p1 = (p_info_t*)malloc(sizeof(struct p_info));
		if(p1 == NULL){
			printf("memory error\n");
			exit(1);
		}
		p1->flag = 0;
		//p1->pre = p;
		p1->next = NULL;
		p->next = p1;

		get_process_info(dir, p1, n);
		return 0;
	}
}
void reset_all_list(p_info_t *head)
{
	p_info_t *p = head;

	while(p->next){
		p->flag = 0;
		p = p->next;
	}
}

//int process_info_main_f()
int main()
{
	DIR *dp ;
	struct dirent* entry;
	struct stat statbuf;
	const char *dir = "/proc/";
	p_info_t *head ;
	int id = 0;

	/*初始化头节点*/
	head = (p_info_t *)malloc(sizeof(struct p_info));
	if(head == NULL){
		printf("memory error\n");
		exit(1);
	}
	head->flag = 0;
	//head->pre = NULL;
	head->next = NULL;
	head->id = -1;

	if( chdir(dir) == -1){
		printf("cannot change the dir\n");
		exit(1);
	}

	/*第一遍读*/
	if( (dp = opendir(dir)) == NULL){
		printf("cannot open the dir %s\n", dir);
		exit(1);
	}
	while((entry = readdir(dp)) != NULL){
		lstat(entry->d_name, &statbuf);
		if(S_ISDIR(statbuf.st_mode)){
			if( (id = atoi(entry->d_name)) != 0){
				/*已经进入某个进程，可以进入该文件夹*/
				/*opendir(entry->d_name) 进入目录*/
				/*chdir() 改变当前目录*/
				/*当结束n时，将当前目录转会 ..*/
				//printf("ID :%d\n", id);
				complete_link_list(head, entry->d_name, id, 1);
			}
		}
	}
	sleep(1);
	/*第二遍读*/
	if( (dp = opendir(dir)) == NULL){
		printf("cannot open the dir %s\n", dir);
		exit(1);
	}
	while((entry = readdir(dp)) != NULL){
		lstat(entry->d_name, &statbuf);
		if(S_ISDIR(statbuf.st_mode)){
			if( (id = atoi(entry->d_name)) != 0){
				/*已经进入某个进程，可以进入该文件夹*/
				/*opendir(entry->d_name) 进入目录*/
				/*chdir() 改变当前目录*/
				/*当结束n时，将当前目录转会 ..*/
				complete_link_list(head, entry->d_name,id, 2);
				//printf("ID :%d\n", id);
			}
		}
	}
	printall(head);
	//sort_by_cpu(head);

	return 0;
}
