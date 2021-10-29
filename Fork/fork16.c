#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void fork16(){
	for(int i=0;i<2;i++){
		if(fork()==0){
			printf("Child%d: pid=%d pgrp=%d\n", i+1, getpid(), getpgrp());
			while(1);
		}
	}

}

int main(){
	fork16();
	return 0;
}
