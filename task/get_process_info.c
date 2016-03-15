#include "task.h"
int printall(p_info_t *p);
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

	pmax_head->cpu_total1 = 0;
	p1 = pmax_head;
	p2 = p1;

	while(1){
		p = phead;
		while(p!=NULL && p->cpu_total1 == 0)
		  p = p->next;
		if(!p)
		  break;
		cpu = p->cpu;
		p1 = p;

		while(p){
			if(p->cpu_total1!=0 && p->cpu > cpu){
				cpu = p->cpu;
				p1 = p;
			}
			p = p->next;
		}
		p1->cpu_total1 = 0;
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

	pmax_head->cpu_total1 = 0;
	p1 = pmax_head;
	p2 = p1;

	while(1){
		p = phead;
		while(p!=NULL && p->cpu_total1 == 0)
		  p = p->next;
		if(!p)
		  break;
		mem = atof(p->mem);
		p1 = p;

		while(p){
			if(p->cpu_total1!=0 && atof(p->mem) > mem){
				mem = atof(p->mem);
				p1 = p;
			}
			p = p->next;
		}
		p1->cpu_total1 = 0;
		p2->p = p1;
		p2 = p1;
	}
	p1->p = NULL;
	return pmax_head;
}
/*
 * 完成对链表的添加
 */
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
/*
 * 将链表标志位清零
 */
void reset_all_list(p_info_t *head)
{
	p_info_t *p = head;

	while(p->next){
		p->flag = 0;
		p = p->next;
	}
}
/*
 * 控制完成对相应信息的读取
 */
int process_info_main_f(p_info_t **head, int n)
{
	DIR *dp ;
	struct dirent* entry;
	struct stat statbuf;
	const char *dir = "/proc/";
	//p_info_t *head ;
	int id = 0;

	/*初始化头节点*/
	if((*head) == NULL){/*如果是第一次则完成初始化*/
		*head = (p_info_t *)malloc(sizeof(struct p_info));
		if(head == NULL){
			printf("memory error\n");
			exit(1);
		}
		(*head)->flag = 0;
		//head->pre = NULL;
		(*head)->next = NULL;
		(*head)->id = -1;
	}

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
				//printf("ID :%d\n", id);
				complete_link_list(*head, entry->d_name, id, n);
			}
		}
	}
	//sleep(1);
	/*第二遍读*/
	/*
	if( (dp = opendir(dir)) == NULL){
		printf("cannot open the dir %s\n", dir);
		exit(1);
	}
	while((entry = readdir(dp)) != NULL){
		lstat(entry->d_name, &statbuf);
		if(S_ISDIR(statbuf.st_mode)){
			if( (id = atoi(entry->d_name)) != 0){
				complete_link_list(*head, entry->d_name,id, 2);
				//printf("ID :%d\n", id);
			}
		}
	}*/
	//printall(*head);
	//sort_by_cpu(head);
	return 0;
}
/*
 * 完成用户组的提取
 */
int get_user_f(p_info_t *head, u_name_to_id_t* head1)
{
	p_info_t *p = head;

	while(p){
		user_find_f(p->user, p->c_user, head1);
		p = p->next;
	}
	return 0;
}
/*
 * 完成将结构体种的数据转换成字符串
 */
int struct_data_to_string(p_info_t *head)
{
	p_info_t *p = head;
	while(p){
		snprintf(p->c_id, 6, "%d", p->id);
		snprintf(p->c_cpu, 7, "%.2f", p->cpu);
		snprintf(p->c_nice, 4, "%d", p->nice);
		snprintf(p->c_priority, 5, "%d", p->priority);
		
		(p->list)[0] = p->name;
		(p->list)[1] = p->c_user;
		(p->list)[2] = p->c_cpu;
		(p->list)[3] = p->c_id;
		(p->list)[4] = p->mem;
		(p->list)[5] = p->c_priority;
		(p->list)[6] = p->c_nice;
		
		p = p->next;
	}

	return 0;
}
void print_cpu_sort(p_info_t *head)
{
	p_info_t *p = head;
	while(p){
		//printf("%s %d %s %lu %s\n", p->name, p->id, p->mem, p->process1, p->list[1]);
		printf("%s %s %s %s %s %s %s \n", p->list[0], p->list[1], p->list[2], p->list[3], p->list[4], p->list[5], p->list[6]);
		p = p->p;
	}
}
int printall(p_info_t *p)
{
	struct p_info s;
	p_info_t * p1 = NULL;
	//p1 = sort_by_cpu(p);
	p1 = sort_by_mem(p);
	//p1 = sort_by_name(p);
	
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
	//getchar();

	print_cpu_sort(p1);

	return 0;
}
/*
int main()
{
	p_info_t *head = NULL;
	u_name_to_id_t *head1 = NULL;

	user_find_list(&head1);
	process_info_main_f(&head, 1);
	sleep(1);
	process_info_main_f(&head, 2);
	get_user_f(head, head1);
	struct_data_to_string(head);
	printall(head);
	
	while(1){
		process_info_main_f(&head, 1);
		sleep(1);
		process_info_main_f(&head, 2);
		get_user_f(head, head1);
		struct_data_to_string(head);
		printall(head);
		getchar();
	}
	
	return 0;
}*/

	
