#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void cleanup(){
	printf("cleaning up\n");
}

void fork6(){
	atexit(cleanup);
	fork();
	exit(0);
}

int main(){
	fork6();
	return 0;
}

