#ifndef __TASK_H_
#define __TASK_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*get_process_info.c*/
typedef struct p_info{
	int id;
	int flag;
	int user;
	long cpu_total1;
	long cpu_total2;
	long process1;
	long process2;
	float cpu;
	int priority;
	int nice;
	char c_id[6];
	char c_user[20];
	char c_cpu[7];
	char c_priority[5];
	char c_nice[4];
	char name[100];
	char mem[50];
	char *list[7];
	struct p_info *p;
	struct p_info *next;
}p_info_t;

int get_process_info(const char* dir, struct p_info *p, int n);
p_info_t* sort_by_name(p_info_t *head);
p_info_t* sort_by_cpu(p_info_t *head);
p_info_t* sort_by_mem(p_info_t *head);
int complete_link_list(p_info_t *head, const char* dir, int id, int n);
int process_info_main_f(p_info_t **head, int n);
void reset_all_list(p_info_t *head);

/*mem.c*/
float mem_f(int *all);

/*hostname.c*/
int hostname(char *name);

/*cpu_name.c*/
int read_cpu_info(char *name);

/*user.c*/
typedef struct user_name_to_id{
	char name[128];
	int id;
	struct user_name_to_id *next;
}u_name_to_id_t;
int user_find_f(int id, char *name, u_name_to_id_t*head);
int user_find_list(u_name_to_id_t **head);

/*cpu.c*/
float cpu_f(int *all, int *idle, int n);

/*version.c*/
int version_f(char *version);

/*start_time.c*/
int start_time(char* run, char* start);

#endif
