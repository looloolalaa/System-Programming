#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

void sig_handler(int signum){
	for(int i=0;i<2;i++){
		printf("this is signal handler\n");
		sleep(2);
	}
}
void* thread1(void *data){
        while(1){
                printf("thread 1\n");
                sleep(2);
        }
}
void* thread2(void *data){
        while(1){
                printf("thread 2\n");
                sleep(2);
        }
}
void* thread3(void *data){
        while(1){
                printf("thread 3\n");
                sleep(2);
        }
}


int main(){
	pthread_t tid[3];
	int thr_ret;
	signal(SIGUSR1, sig_handler);

	thr_ret = pthread_create(&tid[0], NULL, thread1, NULL);
	thr_ret = pthread_create(&tid[1], NULL, thread2, NULL);
	thr_ret = pthread_create(&tid[2], NULL, thread3, NULL);
	sleep(3);

	pthread_kill(tid[1], SIGUSR1);

	sleep(10);
	printf("Main return\n");
	return 0;

}
