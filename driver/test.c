#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#define BUFF_SIZE 256

int write_something(int fd, char *buf)
{
	int err = -1;

	printf("please input something :\n");
	scanf("%s", buf);
	
	if(strlen(buf) > BUFF_SIZE){
		printf("Input error\n");
		exit(1);
	}

	err = write(fd, buf, BUFF_SIZE);
	if(err != BUFF_SIZE){
		printf("write error\n");
		return -1;
	}
	return 0;
}

int read_something(int fd, char *buf)
{
	int err = -1;
	err  = read(fd, buf, BUFF_SIZE);
	if(err != BUFF_SIZE){
		printf("read error\n");
		return -1;
	}

	printf("read : %s\n", buf);
	return 0;
}

int main()
{
	int fd;
	char buf[BUFF_SIZE] = "";
	int err = -1;

	fd = open("/dev/printer", O_RDWR, S_IRUSR|S_IWUSR);
	if(fd != -1){
		/*错误测试,即没有写入东西,应该是不能读取数据*/
		printf("第一次读,但本身就没有东西,所以应该 读出错\n");
		read_something(fd, buf);

		write_something(fd, buf);
		write_something(fd, buf);
		printf("上次写入的东西并没有取出,所以应该是 写出错\n");
		read_something(fd, buf);
		//关闭“/dev/printer”
		close(fd);
	}
	else {
		printf("Device open failure\n");
	}
	return 0;
}

