#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#define LINE 6
#define MAX_THREAD 100

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int a1, a2, thread_count;
char* input_name;
char* output_name;

typedef struct {
	int start;
	int lines;
} Arg;

int digit(int n) {
	int count = 0;
	while (n != 0) {
		n /= 10;
		count++;
	}
	return count;
}

void itoa(int num, char* line) {
	int dig = digit(num);
	int divide = 1;
	for (int i = 4; i >= LINE-1-dig; i--) {
		int n = (num / divide) % 10;
		line[i] = n + '0';
		divide *= 10;
	}
}

void* read_and_write(void *data){
	Arg arg = *((Arg *)data);
	int start = arg.start;
	int lines = arg.lines;
	char line[LINE];
	int num;

	int fd_read = open(input_name, O_RDONLY);
	if ( fd_read < 0 ){
		perror("open_read");
		exit(1);
	}
	lseek(fd_read, start*LINE, SEEK_SET);

	int fd_write = open(output_name, O_WRONLY|O_APPEND);
	if ( fd_write < 0 ){
		perror("open_write");
		exit(1);
	}

	long count = 0;	
	int ret;
	for(int i=0;i<lines;i++){
		ret = read(fd_read, line, sizeof(line));
		if ( ret < 0 ){
			if (errno==EINTR)
				continue;
			perror("read");
			close(fd_read);
			exit(1);
		}
		num = atoi(line);
		if(a1<=num && num<=a2){
			ret = write(fd_write, line, LINE);
			if ( ret < 0 ){
				perror("write");
				close(fd_write);
				exit(1);
			}
			count++;
		}
	}

	close(fd_read);
	close(fd_write);
	return (void*)count;
		
}
	

void write_total_count(int total_count){
	int fd = open(output_name, O_WRONLY);
	if ( fd < 0 ){
		perror("open_write");
		exit(1);
	}
	lseek(fd, 0, SEEK_SET);

	char line[LINE+1] = "     \n";
	itoa(total_count, line);
	int ret = write(fd, line, LINE);
	if ( ret < 0 ){
		perror("write");
		close(fd);
		exit(1);
	}
	close(fd);
}

int init(){
	char line[LINE];
	int fd = open(input_name, O_RDONLY);
	if ( fd < 0 ){
		perror("open_read");
		exit(1);
	}
	int ret = read(fd, line, sizeof(line));
	if ( ret < 0 ){
		perror("read");
		close(fd);
		exit(1);
	}
	close(fd);
	
	fd = open(output_name, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	if ( fd < 0 ){
		perror("open_write");
		exit(1);
	}
	ret = write(fd, "    0\n", LINE);
	if ( ret < 0 ){
		perror("write");
		close(fd);
		exit(1);
	}
	close(fd);
	
	return atoi(line);
}

int main(int argc, char* argv[]){

	if(argc != 6){
		printf("invalid argument\n");
		return 0;
	}

	a1 = atoi(argv[1]), a2 = atoi(argv[2]);
	thread_count = atoi(argv[3]);
	input_name = argv[4];
	output_name = argv[5];

	if(a1 > a2 || thread_count <= 0){
		printf("invalid argument\n");
		return 0;
	}
	

	int input_count = init();
	if (input_count < thread_count) thread_count = input_count;
	int lines_per_thread = input_count / thread_count;
	int lines = lines_per_thread;


	Arg arg[MAX_THREAD];
	pthread_t tid[MAX_THREAD];
	int status;
	for(int i=0;i<thread_count;i++){
		arg[i].lines = lines;

		if ( i==thread_count-1 ) 
			arg[i].lines = input_count-(lines_per_thread)*(thread_count-1);

		arg[i].start = i*lines_per_thread+1;
		status = pthread_create(&tid[i], NULL, read_and_write, (void*)&(arg[i]));
		if (status != 0){
			perror("pthread_create");
			exit(1);
		}
		

	}

	int part_sum[MAX_THREAD];
	int total_count = 0;
	for(int i=0;i<thread_count;i++){
		status = pthread_join(tid[i], (void*)&part_sum[i]);
		if (status != 0){
			perror("pthread_join");
			exit(1);
		}

		pthread_mutex_lock(&mutex);
		total_count += part_sum[i];
		pthread_mutex_unlock(&mutex);

	}
		
	
	printf("%d\n", total_count);
	write_total_count(total_count);
	
	pthread_mutex_destroy(&mutex);
	return 0;
}

