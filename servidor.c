#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(int argc,char const *argv[]){
	//Abrir o pipe que recebe informação do cliente
	int fd_in;
	if((fd_in = open("Cliente",O_RDONLY)) < 0){
		perror("open cliente");
		exit(1);
	}
	//Abrir o pipe que envia informação ao cliente
	int fd_out;
	if((fd_out = open("Bus",O_WRONLY)) < 0){
		perror("open bus");
		exit(1);
	}
	char buf[100];
	int bytes_read = 0;
	while((bytes_read = read(fd_in,buf,100)) > 0){
		write(1,buf,bytes_read);
	}

	close(fd_in);
	close(fd_out);
	return 0;
}
