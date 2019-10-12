#include<stdlib.h>
#include<stdio.h>

int main(int argc, char* argv[], char* env) {
	float a;
	float b;
	sscanf(argv[1], "%f", &a);
	sscanf(argv[2], "%f", &b);
	
	if(a >= b)
	printf("1");
	else
	printf("0");

	return 0;
}
