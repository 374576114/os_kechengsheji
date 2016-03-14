#include <stdlib.h>
#include <stdio.h>

int main()
{
	char *p = "1.23 M";
	float m;

	m  = atof(p);
	printf("%f", m);
	return 0;
}
