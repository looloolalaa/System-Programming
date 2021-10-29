#include "ku_ff_input.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <mqueue.h>

#define MSG_SIZE 4
#define NAME "/m_queue"

int main(int argc, char* argv[]){

	//time
	struct timeval start, end, gep;
	gettimeofday(&start, NULL);

	int a1 = atoi(argv[1]), a2 = atoi(argv[2]), i = atoi(argv[3]);
	if(argc != 4 || a1 > a2 || i <= 0){
		printf("invalid argument\n");
		return 0;
	}
	
	//common

	pid_t pid[i];
	struct mq_attr attr;
	int value = -1;
	unsigned int prio = 1;
	mqd_t mqdes;

	attr.mq_maxmsg = 10;
	attr.mq_msgsize = MSG_SIZE;

	mqdes = mq_open(NAME, O_CREAT|O_RDWR, 0600, &attr);
	if( mqdes < 0 ){
		perror("mq_open()");
		exit(0);
	}
	

	// children

	int child_count = i;
	int term = (child_count <= NUMS)? (NUMS / child_count): 1;
	for(int i = 0 ; i < child_count ; i++){

		int start = i*term;
		int end = (i == child_count-1 && child_count <= NUMS)? NUMS-1: start+term-1;

		if((pid[i] = fork()) == 0){
			//for(int k=0;k<200000000;k++)
			//	;
			if( start > NUMS-1 )
				exit(0);

			int count = 0;
			for(int i=start;i<=end;i++)
				if(a1<=input[i] && input[i]<=a2)
					count++;
			
			if( mq_send(mqdes, (char*)&count, MSG_SIZE, prio) == -1 ){
				perror("mq_send()");
				exit(0);
			}
			//printf("send!\n");

			exit(100+i);
		}
	}

	
	//parent

	int status;
	int all_count = 0;
	int msg_count = (child_count > NUMS)? NUMS: child_count;
	for(int i = 0 ; i < child_count ; i++){
		pid_t wpid = wait(&status);
		if(!(pid[0]<=wpid && wpid<=pid[msg_count-1]))
			continue;
		//if(WIFEXITED(status))
		//	printf("child exit success: %d process(%d)\n", wpid, WEXITSTATUS(status));
		//else
		//	printf("child exit fail: %d process\n", wpid); 

		if( mq_receive(mqdes, (char*)&value, MSG_SIZE, &prio) == -1 ){
			perror("mq_receive()");
			exit(0);
		}
		//printf("receive!: (prio: %d, val: %d)\n", prio, value);
		all_count += value;
	}

	mq_close(mqdes);
	mq_unlink(NAME);

	//printf("all children return & c0: %d\n", all_count);
	printf("%d\n", all_count);

	//for(int i=0;i<2000000000;i++)
	//	;
	
	gettimeofday(&end, NULL);
	gep.tv_sec = end.tv_sec - start.tv_sec;
	gep.tv_usec = end.tv_usec - start.tv_usec;
	if (gep.tv_usec < 0){
		gep.tv_sec = gep.tv_sec - 1;
		gep.tv_usec = gep.tv_usec + 1000000;
	}

	
	//FILE* f = fopen("time_2billion.txt", "a");
	//if(f==NULL)
	//	printf("file error\n");
	//else{
	//	fprintf(f, "%d %02ld.%06ld\n", child_count, gep.tv_sec, gep.tv_usec);
	//	fclose(f);
	//}
	//printf("time: [%02ld.%06ld] sec\n",gep.tv_sec, gep.tv_usec);

	return 0;
}
