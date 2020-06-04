#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int printHelp(){
	int i = 0;
	char *buf[1024];

	int fd = open("help.txt", O_CREAT | O_WRONLY, 0600);

	while (i = read(fd,buf,1024)){
        bytes += i;     ;
    }

	close(fd);

	write(1,buf,i);

	return 0;
}
