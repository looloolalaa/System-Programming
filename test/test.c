#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>

typedef struct {
	int start;
	int lines;
} Arg;

void* main_thread(void *data){
	Arg arg = *((Arg *)data);
	long count = arg.start+arg.lines;
	return (void*)(count);
}


int main(){
	pthread_t tid;
	int result;

	Arg arg;
	arg.start = 1;
	arg.lines = 4;
	
	int total = 0;
	for(int i=0;i<3;i++){
		arg.start= i+1;
		arg.lines = 4;
		int status = pthread_create(&tid, NULL, main_thread, (void*)&(arg));
		status = pthread_join(tid, (void*)&result);
		printf("%d\n", result);
		total += result;
	}
	printf("total: %d\n", total);


	return 0;
}
