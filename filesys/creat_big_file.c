#include "file.h"

#define BIG_FILE_SIZE (1024*1024*1)
#define FREE_SIZE (sizeof(free_b_list_t))
#define INDEX_SIZE (sizeof(index_info_t))
#define ALL_INFO_SIZE (sizeof(fill_all_info_t))

/*空闲文件块列表*/
typedef struct file_free_block_list{
    unsigned long offset;
    unsigned long size;
    struct file_free_block_list * next;
}free_b_list_t;

/*文件基本信息*/
typedef struct index_info{
    char name[128];
    unsigned long inode;
}index_info_t;

/*文件信息头，inode对应信息*/
typedef struct file_all_info{
    char type;
    time_t change_time;
    unsigned short rwx;
    unsigned long offset_this;
    unsigned long size;
    unsigned long offset_next;
}fill_all_info_t;

int main()
{
    int fd;
    free_b_list_t *head = NULL;

    fd = open("./big.disk", O_RDWR | O_CREAT, 0666);
    if(fd == -1){
	printf("open file failed\n");
	exit(1);
    }

    close(fd);
    return 0;
}

/*将free_block写回磁盘*/
int f_write_free_block_list(free_b_list_t *head, int fd)
{
    free_b_list_t *p = head;

    lseek(fd, 0, SEEK_SET);
    while(p){
	write(fd, p, FREE_SIZE);
	p = p->next;
    }

    return 0;
}



    
    

