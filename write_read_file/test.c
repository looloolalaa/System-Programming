#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
	float a = 44;
	int fd = open("test.txt", O_RDWR|O_CREAT|O_TRUNC);
	write(fd, &a, sizeof(a));
	close(fd);

	return 0;
}

