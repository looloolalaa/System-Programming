#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main(){
	int len = 10;
	char num[10];
	char str[10];
	char buf[11];
	int fd = open("table.txt", O_RDWR|O_CREAT|O_TRUNC);
	off_t pos = 10;
	int ret;

	while(1){
		scanf("%s %s", num, str);
		if ( atoi(num) < 0 ) break;
		if( fork() == 0 ){
			write(fd, num, strlen(num));
			lseek(fd, len-strlen(num), SEEK_CUR);
			write(fd, " ", 1);
			write(fd, str, strlen(str));
			lseek(fd, len-strlen(str), SEEK_CUR);
			write(fd, "\n", 1);
			exit(0);
		}
		else{
			wait(NULL);
			//lseek(fd, 0, SEEK_SET);
			//while( read(fd, buf, strlen(buf))!= 0 ){
			//	printf("out: %s\n", buf);
			//}
		}
	}

	lseek(fd, 0, SEEK_SET);
                        while( read(fd, buf, sizeof(buf))!= 0 ){
                                printf("%s\n", buf);
                        }


	close(fd);

	return 0;
}
