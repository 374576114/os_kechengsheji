#include <unistd.h>
#include <stdio.h>

int main(){
	int a = 0;
	int i = 0;

	for(i=1; i<101; i++){
		a += i;
		printf("%d\n", a);
		sleep(1);
	}

	return 0;
}
