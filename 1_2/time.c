#include <unistd.h>
#include <stdio.h>
#include <time.h>

int main(){
	time_t t;

	while(1){
		time(&t);
		printf("%s\n", ctime(&t));
		sleep(1);
	}
	return 0;
}
