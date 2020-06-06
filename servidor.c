#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(int argc,char const *argv[]){
	int fd;
	fd = open("named_pipe",O_RDONLY);

	char buf[100];
	int bytes_read = 0;
	while((bytes_read = read(fd,buf,100)) > 0){
		write(1,buf,bytes_read);
	}

	close(fd);
	return 0;
}
