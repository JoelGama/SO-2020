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
	while(1);
	return 0;
}
