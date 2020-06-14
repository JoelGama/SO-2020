#include "argus.h"

int main(int argc, char *argv[]){
	// Abrir o pipe que envia informação ao servidor
	int fd_out;
	if((fd_out = open(CLIENTE,O_WRONLY)) < 0){
		perror("open cliente");
		exit(1);
	}
	// Abrir o pipe que recebe informação do servidor
	int fd_in;
	if((fd_in = open(BUS,O_RDONLY)) < 0){
		perror("open bus");
		exit(1);
	}

	char buf[SIZE_S];
	char *buffer = NULL;
	int read_bytes;
	
	// Se o cliente não passar argumentos, executar a shell 
	if(argc == 1){
		write(1,"\e[1;1H\e[2J",11);


		write(1,"argus$ ",7);

		while((read_bytes = read(0,buf,SIZE_S))>0){
			if(write(fd_out,buf,read_bytes) < 0){
				perror("write");
				exit(1);
			}
			buffer = (char *) malloc(SIZE_L * sizeof(char));
			if((read_bytes = read(fd_in, buffer, SIZE_L)) > 0){
				write(1, buffer, read_bytes);
			}
			write(1,"argus$ ",7);
		}
	}
	// Caso contrário, executar o comando dado pelo cliente
	else{
		int i;
		char comando[SIZE_S];
		for(i=1 ; i<argc ; i++){
			strcat(comando, argv[i]);
			strcat(comando, " ");
		}

		if(write(fd_out,comando,strlen(comando)+1) < 0){
			perror("write");
			exit(1);
		}
		buffer = (char *) malloc(SIZE_L * sizeof(char));
		if((read_bytes = read(fd_in, buffer, SIZE_L)) > 0){
			write(1, buffer, read_bytes);
		}
	}

	close(fd_in);
	close(fd_out);
	return 0;
}