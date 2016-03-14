#include <gtk/gtk.h>
 

void select_row_callback(GtkWidget *clist,gint row,gint column,GdkEventButton *event,gpointer data);

 

int main(int argc,char *argv[ ])

{

GtkWidget *window;

GtkWidget *clist;

gchar *text1[3]={"刘备","男","23"}; /*定义列表项数据*/

gchar *text2[3]={"关羽","男","22"};

 

gtk_init(&argc,&argv);

window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_signal_connect(GTK_OBJECT(window),"delete_event",GTK_SIGNAL_FUNC(gtk_main_quit),NULL);

gtk_widget_set_size_request(window,200,150);

gtk_container_set_border_width(GTK_CONTAINER(window),10);

 

clist=gtk_clist_new(3);

gtk_clist_set_column_title(GTK_CLIST(clist),0,"姓名");

gtk_clist_set_column_title(GTK_CLIST(clist),1,"姓别");

gtk_clist_set_column_title(GTK_CLIST(clist),2,"年龄");

 

gtk_clist_prepend(GTK_CLIST(clist),text1);/*将列表项数据添加到列表*/

gtk_clist_prepend(GTK_CLIST(clist),text2);

 

gtk_signal_connect(GTK_OBJECT(clist),"select_row",GTK_SIGNAL_FUNC(select_row_callback),NULL);/*捕捉select_row信号*/

 

gtk_clist_column_titles_show(GTK_CLIST(clist));

gtk_container_add(GTK_CONTAINER(window),clist);

 

gtk_widget_show(clist);

gtk_widget_show(window);

gtk_main();

}

 

void select_row_callback(GtkWidget *clist,gint row,gint column,GdkEventButton *event,gpointer data)/*设置select_row信号的回调函数*/

{

gchar *text=NULL;

gtk_clist_get_text(GTK_CLIST(clist),row,column,&text);

//gprint("%s/n",text);

}
