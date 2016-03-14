#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

/*add 全局变量*/
int sum = 0;
int i = 0;
GtkWidget *label_add;
/*cpu 全局变量*/
GtkWidget *label_cpu;
/*time 全局变量*/
GtkWidget *label_time;

/*
 *功能:完成1到100的累加
 */
gint addf(gpointer data){
	char str[20] = "";

	i++;
	sum += i;

	sprintf(str, "sum: %d", sum);
	gtk_label_set_text(GTK_LABEL(label_add), str);

	return TRUE;
}
/*
 *功能: 完成cpu利用率的计算
 *
 */
gint cpuf(gpointer data){
	FILE *fp = NULL;
	char name[5] = "";
	char buff[30] = "";
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
	if(fp == NULL)
	  exit(1);
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
//	printf("%.3f %.3f cpu : %.3f%%\n", a, idle, f);
	sprintf(buff, "cpu : %.3f%%", f);
	gtk_label_set_text(GTK_LABEL(label_cpu), buff);
	return TRUE;
}
/*
 * 功能:完成时间的显示
 */
gint timef(gpointer data){
	time_t t;
	char str[30] = "";

	time(&t);
	sprintf(str, "%s", ctime(&t));
	gtk_label_set_text(GTK_LABEL(label_time), str);

	return TRUE;
}
/*
 * 功能:显示累加和窗口
 */
int addw(int *argc, char **argv[], int t){
	GtkWidget *window, *frame;
	
	gtk_init(argc, argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	//gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER_ON_PARENT );
	gtk_window_set_default_size(GTK_WINDOW(window), 200, 180);
	gtk_window_set_title(GTK_WINDOW(window), "1到100累加");
	//gtk_window_reposition(GTK_WINDOW(window), 0, 0);
	//gtk_widget_set_uposition(GTK_WINDOW(window), 0, 0);
	gtk_window_move((GtkWindow *)window, 300,100);

	frame = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window), frame);

	label_add = gtk_label_new("sum: 0");
	gtk_fixed_put(GTK_FIXED(frame), label_add, 80, 76);

	gtk_widget_show_all(window);

	g_timeout_add(t, addf, NULL);

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_main();
	
	return 0;
}

/*
 * 功能:显示cpu利用率窗口
 */
int cpuw(int *argc, char **argv[], int t){
	GtkWidget *window, *frame;
	
	gtk_init(argc, argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	//gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER_ON_PARENT );
	gtk_window_set_default_size(GTK_WINDOW(window), 200, 180);
	gtk_window_set_title(GTK_WINDOW(window), "cpu 利用率");
	gtk_window_move((GtkWindow*)window, 500,100);

	frame = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window), frame);

	label_cpu = gtk_label_new("cpu :0.00%");
	gtk_fixed_put(GTK_FIXED(frame), label_cpu, 40, 80);

	gtk_widget_show_all(window);

	g_timeout_add(t, cpuf, NULL);

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_main();
	
	return 0;
}
/*
 * 功能:显示时间窗口
 */
int timew(int *argc, char **argv[], int t){
	GtkWidget *window, *frame;
	
	gtk_init(argc, argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	//gtk_window_set_position(GTK_WINDOW(window),NONE);
	gtk_window_set_default_size(GTK_WINDOW(window), 200, 180);
	gtk_window_set_title(GTK_WINDOW(window), "时间");
	gtk_window_move((GtkWindow*)window, 100,100);

	frame = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window), frame);

	label_time = gtk_label_new("-----");
	gtk_fixed_put(GTK_FIXED(frame), label_time, 10, 80);

	gtk_widget_show_all(window);

	g_timeout_add(t, timef,  NULL);

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_main();
	
	return 0;
}

int main(int argc , char *argv[]){
	pid_t pid1, pid2;

	pid1 = fork();
	if(pid1 < 0){
		printf("can not create process \n");
		exit(1);
	}else if(pid1 == 0){
		cpuw(&argc, &argv, 2000);
	}else{
		pid2 = fork();
		if(pid2<0){
			printf("can not create process \n");
			exit(1);
		}else if(pid2 == 0){
			addw(&argc, &argv, 3000);
		}else{
			timew(&argc, &argv, 1000);

			waitpid(pid1, NULL, 0);
			waitpid(pid2, NULL, 0);
			return 0;
		}
	}
}
