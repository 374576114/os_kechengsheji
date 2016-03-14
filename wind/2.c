#include <gtk/gtk.h>


void deal_switch_page(GtkNotebook *notebook, gpointer page, guint page_num, gpointer data)
{
	printf("我是第%d个页面\n", page_num+1);
	return;
}

int main(int argc, char *argv[])
{
	//1.gtk环境
	gtk_init(&argc, &argv);

	//2.创建一个window
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	//设置窗口大小
	gtk_widget_set_size_request(window, 400, 300);

	//3.创建笔记本控件
	GtkWidget *notebook = gtk_notebook_new();
	gtk_container_add(GTK_CONTAINER(window), notebook);

	//4.页标签的位置，可以有四种位置：上、下、左或右
	gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_TOP);//上面

	//5.创建一个水平布局
	GtkWidget *hbox = gtk_vbox_new(TRUE, 5);
	gtk_container_add(GTK_CONTAINER(window), hbox);


	//6.第一个页面
	GtkWidget *label = gtk_label_new("Page one");

	GtkWidget *button1 = gtk_button_new_with_label("我是第一个页面");
	gtk_container_add(GTK_CONTAINER(hbox), button1);

	GtkWidget *button2 = gtk_button_new_with_label("我是第一个页面的按钮");
	gtk_container_add(GTK_CONTAINER(hbox), button2);

	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), hbox, label);

	//7.第二个页面
	label = gtk_label_new("Page two");
	hbox = gtk_hbox_new(TRUE, 5);
	button1 = gtk_button_new_with_label("我是第二个页面");
	gtk_container_add(GTK_CONTAINER(hbox), button1);

	button2 = gtk_button_new_with_label("我是第二个页面的按钮");
	gtk_container_add(GTK_CONTAINER(hbox), button2);

	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), hbox, label);

	//8.第三个页面,在指定位置添加页面，从0开始
	label = gtk_label_new("page three");
	button1 = gtk_button_new_with_label("我是通过insert_page实现的，第三页面");
	gtk_notebook_insert_page(GTK_NOTEBOOK(notebook), button1, label, 2);

	//9.处理信号，当切换页面的时候，会触发“switch-page”信号
	g_signal_connect(notebook, "switch-page", G_CALLBACK(deal_switch_page), NULL);
	g_signal_connect(GTK_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);

	//10.设置默认起始页为第二页，从0开始算，那就是1
	gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook),1);

	//11.显示所有网页
	gtk_widget_show_all(window);

	//12.主事件循环
	gtk_main();

	return 0;
}
