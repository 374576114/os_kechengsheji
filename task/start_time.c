#include "task.h"

int start_time(char *run, char *start)
{
	FILE *fp = NULL;
	time_t t_now, t_run, t_start;
	time_t hours, min;
	char c_h[4], c_m[4], c_s[4];

	time(&t_now);

	fp = fopen("/proc/uptime", "r");
	if(fp == NULL){
		printf("can not open the uptime\n");
		strcpy(run, "");
		strcpy(start, "");
		return -1;
	}

	fscanf(fp, "%ld", &t_run);
	t_start = t_now - t_run;

	hours = t_run / 3600;
	t_run %= 3600;
	min = t_run / 60;
	t_run %= 60;
	if(hours < 10)
	  sprintf(c_h, "0%ld", hours);
	else
	  sprintf(c_h, "%ld", hours);

	if(min < 10)
	  sprintf(c_m, "0%ld", min);
	else
	  sprintf(c_m, "%ld", min);
	
	if(t_run < 10)
	  sprintf(c_s, "0%ld", t_run);
	else
	  sprintf(c_s, "%ld", t_run);

	sprintf(run, "%s:%s:%s", c_h, c_m, c_s);

	strcpy(start, ctime(&t_start));

	fclose(fp);
	return 0;
}

int main(){
	char s1[20], s2[50];
	start_time(s1, s2);
	printf("Start Time: %s", s2+11);
	printf("Running Time: %s\n", s1);
	return 0;
}
