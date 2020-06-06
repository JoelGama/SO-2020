#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
	if(argc == 1){
		int fd;
		if((fd = open("named_pipe",O_WRONLY)) < 0){
			perror("open");
			exit(1);
		}

		write(1,"\e[1;1H\e[2J",11);

		char buf[100];
		int read_bytes;

		write(1,"argus$ ",7);

		while((read_bytes = read(0,buf,100))>0){
			write(1,"argus$ ",7);
			if(write(fd,buf,read_bytes) < 0){
				perror("write");
				exit(1);
			}
		}
	}else{
		int i;
		char comando[100]
		for(i=1 ; i<argc ; i++){
			strcat(comando, argv[i]);
			strcat(comando, " ");
		}

		if(write(fd,comando,strlen(comando)+1) < 0){
			perror("write");
			exit(1);
		}
	}
	return 0;
}
