#include "task.h"
#include <gtk/gtk.h>

#define CPU_SORT 1
#define MEM_SORT 2
#define NAME_SORT 3
#define ID_SORT 0

/*全局变量*/
GtkWidget *list;
GtkWidget *l15;
GtkWidget *bar1;
GtkWidget *bar2;
int sort_type = ID_SORT; /*0 id, 1 cpu 2 mem 3 name*/
static ntime = 0;
int all_time=0, idle_time=0;

p_info_t *head1 = NULL;
u_name_to_id_t *head2 = NULL;

/*
 * 设置list
 */
int set_list_style()
{
	gtk_clist_set_column_justification(GTK_CLIST(list), 0, GTK_JUSTIFY_LEFT);
	gtk_clist_set_column_justification(GTK_CLIST(list), 1, GTK_JUSTIFY_RIGHT);
	gtk_clist_set_column_justification(GTK_CLIST(list), 2, GTK_JUSTIFY_RIGHT);
	gtk_clist_set_column_justification(GTK_CLIST(list), 3, GTK_JUSTIFY_RIGHT);
	gtk_clist_set_column_justification(GTK_CLIST(list), 4, GTK_JUSTIFY_RIGHT);
	gtk_clist_set_column_justification(GTK_CLIST(list), 5, GTK_JUSTIFY_RIGHT);
	gtk_clist_set_column_justification(GTK_CLIST(list), 6, GTK_JUSTIFY_RIGHT);

	gtk_clist_set_column_width(GTK_CLIST(list), 0, 120);
	gtk_clist_set_column_width(GTK_CLIST(list), 1, 80);
	gtk_clist_set_column_width(GTK_CLIST(list), 2, 60);
	gtk_clist_set_column_width(GTK_CLIST(list), 3, 60);
	gtk_clist_set_column_width(GTK_CLIST(list), 4, 90);
	gtk_clist_set_column_width(GTK_CLIST(list), 5, 60);
	gtk_clist_set_column_width(GTK_CLIST(list), 6, 40);

	gtk_clist_column_title_passive(GTK_CLIST(list),1); 
	gtk_clist_column_title_passive(GTK_CLIST(list),5); 
	gtk_clist_column_title_passive(GTK_CLIST(list),6); 

	return 0;
}

/*
 * 1s 改变开机时间
 */
gint set_running_time()
{
	char buff1[30], buff2[30];
	char c_running_time[50];
	start_time(buff1, buff2);
	snprintf(c_running_time, 50, "运行时间: %s", buff1);

	gtk_label_set_text(GTK_LABEL(l15), c_running_time);
	return 0;
}
/*
 * 显示Clist
 */
int clist_display(p_info_t *p)
{
	int i = sort_type;
	gtk_clist_freeze(GTK_CLIST(list));
	gtk_clist_clear(GTK_CLIST(list));
	while(p){
		if(p->flag == 2){
			gtk_clist_append(GTK_CLIST(list), p->list);
		}
		if(i == 0)
		      p = p->next;
		else 
		      p = p->p;
	}
	gtk_clist_thaw(GTK_CLIST(list));
	return 0;
}
		
/*
 * 3s 改变一次进程状态
 */
gint set_process_info(int n)
{
	p_info_t *p_sort = NULL;
	/*完成所有信息的读取*/
	if(n == 1){
		process_info_main_f(&head1, n);
		return 0;
	}
	process_info_main_f(&head1, n);
	get_user_f(head1, head2);
	struct_data_to_string(head1);
	/*进行排序*/
	switch(sort_type){
		case 0:
			p_sort = head1;
			break;
		case 1:
			p_sort = sort_by_cpu(head1);
			break;
		case 2:
			p_sort = sort_by_mem(head1);
			break;
		case 3:
			p_sort = sort_by_name(head1);
			break;
		default:
		      printf("sort_type error\n");
		      break;
	}
	/*显示clist*/
	clist_display(p_sort);
	reset_all_list(head1);
	return 0;
}

gint set_mem_bar_info(float f, int all)
{
	float fraction = f/100.0;
	float use = fraction * all / 1024;
	int a = all /1024;
	char buff[30];

	snprintf(buff, 30, "%.2f%%  %.0fMB/%dMB", f, use, a);
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(bar2), buff);
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(bar2), fraction);
	return 0;
}
gint set_cpu_bar_info(float f)
{
	float cpu = f/100.0;
	char buff[30];

	snprintf(buff, 30, "%.2f%%", f);
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(bar1), buff);
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(bar1), cpu);
	return 0;
}
	
/*
 * 定时器时进行的所有操作
 */
gint timeout_f(gpointer data)
{
	float fcpu, fmem;
	int all;
	
	set_running_time();
	fmem = mem_f(&all);
	set_mem_bar_info(fmem, all);

	if((ntime % 2) == 0){/*更新第三个页面*/
		set_process_info(1);
		cpu_f(&all_time, &idle_time, 1);
		//set_cpu_mem_info();
	}else{
		fcpu = cpu_f(&all_time, &idle_time, 2);
		set_process_info(2);
		set_cpu_bar_info(fcpu);
	}
	ntime ++;
	return TRUE;
}
/*
 * column 点击回调函数
 */
void select_column_callback(GtkWidget *list, gint colmn, gpointer data)
{
	p_info_t *p = NULL;
	printf("%d \n", colmn);
	if(colmn == 0){//name
		sort_type = NAME_SORT;
		//p = sort_by_name(head1);
		//clist_display(p);
	}else if(colmn == 2){//cpu
		sort_type = CPU_SORT;
		//p = sort_by_cpu(head1);
		//clist_display(p);
	}else if(colmn == 3){//ID
		sort_type = ID_SORT;
		//clist_display(head1);
	}else if(colmn == 4){//MEM
		sort_type = MEM_SORT;
		//p = sort_by_mem(head1);
		//clist_display(p);
	}
}
		



int main(int argc, char *argv[])
{
	/*初始变量*/
	GtkWidget *window;
	char buff[100], buff1[100];
	gchar *titles[] = {"进程名字","用户", "% CPU", "ID", "VmRss 内存", "priority", "nice"};
	GtkWidget *notebook;

	/*初始化阶段*/
	gtk_init(&argc, &argv);
	user_find_list(&head2);/*初始化 用户组链表*/
	printf("wati 2 sec\n");

	process_info_main_f(&head1, 1);
	sleep(1);
	process_info_main_f(&head1, 2);
	get_user_f(head1, head2);
	struct_data_to_string(head1);
	reset_all_list(head1);


	/*配置整体界面布局*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(window, 600, 400);
	gtk_window_set_title(GTK_WINDOW(window), "任务管理器");
	gtk_widget_set_uposition(window, 100,100);
	gtk_container_border_width(GTK_CONTAINER(window), 15);

	list = gtk_clist_new_with_titles(7, titles);/*clist*/

	notebook = gtk_notebook_new();
	gtk_container_add(GTK_CONTAINER(window), notebook);/*notebook 装进window*/
	gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_TOP);
	
	/*第一个页面*/
	char c_hostname[100];
	char c_version[100];
	char c_cpu[100];
	char c_start_time[100];
	char c_running_time[100];
	hostname(buff);
	snprintf(c_hostname, 100, "主机名: %s ", buff);
	version_f(buff);
	snprintf(c_version, 100, "版本号: %s", buff);
	read_cpu_info(buff);
	snprintf(c_cpu, 100, "CPU: %s", buff);
	start_time(buff, buff1);
	snprintf(c_start_time, 100, "开机时间： %s", buff1);
	snprintf(c_running_time, 100, "运行时间: %s", buff);

	GtkWidget *l11 = gtk_label_new(c_hostname);
	GtkWidget *l12 = gtk_label_new(c_version);
	GtkWidget *l13 = gtk_label_new(c_cpu);
	GtkWidget *l14 = gtk_label_new(c_start_time);
	l15 = gtk_label_new(c_running_time);
	GtkWidget *label_title_1 = gtk_label_new("计算机整体信息");
	GtkWidget *table1 = gtk_table_new(5, 3, FALSE);
	gtk_container_border_width(GTK_CONTAINER(table1), 15);
	gtk_table_attach_defaults(GTK_TABLE(table1), l11, 0, 2, 0, 1);
	gtk_table_attach_defaults(GTK_TABLE(table1), l12, 0, 2, 1, 2);
	gtk_table_attach_defaults(GTK_TABLE(table1), l13, 0, 2, 2, 3);
	gtk_table_attach_defaults(GTK_TABLE(table1), l14, 0, 2, 3, 4);
	gtk_table_attach_defaults(GTK_TABLE(table1), l15, 0, 2, 4, 5);

	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), table1, label_title_1);

	/*第二个页面*/
	GtkWidget *label_title_2 = gtk_label_new("所有进程信息");
	GtkWidget* scrolled = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);

	set_list_style();

	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled), list);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), scrolled, label_title_2);

	/*第三个页面*/
	GtkWidget *label_title_3 = gtk_label_new("整体性能");
	GtkWidget *l31 = gtk_label_new("CPU :");
	GtkWidget *l32 = gtk_label_new("MEM :");
	bar1 = gtk_progress_bar_new();
	bar2 = gtk_progress_bar_new();
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(bar1), 0.32);
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(bar1), "32%");
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(bar2), 0.42);
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(bar2), "32%");

	GtkWidget *table2 = gtk_table_new(5, 5, TRUE);
	gtk_container_border_width(GTK_CONTAINER(table2), 10);
	gtk_table_attach_defaults(GTK_TABLE(table2), l31, 1, 2, 1, 2);
	gtk_table_attach_defaults(GTK_TABLE(table2), l32, 1, 2, 3, 4);
	gtk_table_attach_defaults(GTK_TABLE(table2), bar1, 2, 4, 1, 2);
	gtk_table_attach_defaults(GTK_TABLE(table2), bar2, 2, 4, 3, 4);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), table2, label_title_3);
	
	/*显示*/
	gtk_widget_show_all(window);

	/*所有信号处理*/
	gtk_signal_connect(GTK_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_timeout_add(1000, timeout_f, NULL);
	gtk_signal_connect(GTK_OBJECT(list), "click_column", GTK_SIGNAL_FUNC(select_column_callback), NULL);
	gtk_main();
}
