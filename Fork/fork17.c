#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void fork17(){
	if(fork()==0){
		printf("Child: pid=%d pgrp=%d\n", getpid(), getpgrp());
		while(1);
	}
	else{
		printf("Parent: pid=%d pgrp=%d\n", getpid(), getpgrp());
		while(1);
	}
}

int main(){
	fork17();
	return 0;
}
