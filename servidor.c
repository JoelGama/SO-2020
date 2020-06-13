#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "auxiliares.h"


int main(int argc,char const *argv[]){
	static int tempo_execucao = 0;
	static int tempo_inatividade = 0;
	static int indice_tarefa = 0;
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
	char** comando;
	while(1){
		while((bytes_read = read(fd_in,buf,100)) > 0){
			comando = split(buf," ");
			
			if(strcmp(cmd[0],"tempo-execucao") == 0){
				tempo_execucao = atoi(cmd[1]);
				printf("%d\n",tempo_execucao);				
			}
			else if(strcmp(cmd[0],"tempo-inatividade") == 0){
				tempo_inatividade = atoi(cmd[1]);
				printf("%d\n",tempo_inatividade);
			}
			else if(strcmp(cmd[0],"ajuda") == 0){
				printHelp();
			}
			else if(strcmp(cmd[0],"listar") == 0){
				write(1,"To do",5);
			}
			else if(strcmp(cmd[0],"historico") == 0){
				write(1,"To do",5);
			}
			else if(strcmp(cmd[0],"executar") == 0){
				// executar a tarefa
				indice_tarefa++;
			}
			else if(strcmp(cmd[0],"terminar") == 0){
				write(1,"To do",5);
			}
			else{
				write(1,"invalido\n",9);
			}

		}
	}

	close(fd_in);
	close(fd_out);
	return 0;
}
