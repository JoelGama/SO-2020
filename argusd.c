#include "argus.h"

#define EM_EXECUCAO 0
#define TERMINADAS 1

/**
 * ######Log_State######
 * 0 -> Inacabado
 * 1 -> Acabado
 * 2 -> Killed by tempo-execucao
 * 3 -> Killed by tempo-inatividade
 * 4 -> Killed by user
 * 
 * id status 'comando'
**/

int main(int argc,char const *argv[]){
	static int tempo_execucao = 0;
	//static int tempo_inatividade = 0;
	static int indice_tarefa = 0;
	//Abrir o pipe que recebe informação do cliente
	int fd_in;
	if((fd_in = open(CLIENTE,O_RDONLY)) < 0){
		perror("open cliente");
		exit(1);
	}
	//Abrir o pipe que envia informação ao cliente
	int fd_out;
	if((fd_out = open(BUS,O_WRONLY)) < 0){
		perror("open bus");
		exit(1);
	}

	int fd_lixo;
	if((fd_lixo = open("output.txt", O_CREAT | O_TRUNC | O_RDWR, 0660)) < 0){
		perror("open output");
		exit(1);
	}
	dup2(fd_lixo, 1);
	close(fd_lixo);

	int fd_log;
	if((fd_log = open("log.txt", O_CREAT | O_TRUNC | O_RDWR, 0660)) < 0){
		perror("open log");
		exit(1);
	}

	int fd_pids;
	if((fd_pids = open("pids.txt", O_CREAT | O_TRUNC | O_RDWR, 0660)) < 0){
		perror("open pids");
		exit(1);
	}

	char buf[SIZE_S];
	int bytes_read = 0;
	char** comando;
	char msg[SIZE_L] = "";
	char aux[4] = "";

	while(1){
		while((bytes_read = read(fd_in,buf,SIZE_S)) > 0){
			comando = splitComando(buf);
			msg[0] = '\0';
			
			if(strcmp(comando[0],"tempo-execucao") == 0){
				if(comando[1] == NULL)write(fd_out, "Falta informacao!\n", 18);
				else{
					tempo_execucao = atoi(comando[1]);
					strcat(msg, "Tempo de execucao alterado para: ");
					strcat(msg, comando[1]);
					strcat(msg, "!\n");
					write(fd_out, msg, strlen(msg));
				}				
			}
			else if(strcmp(comando[0],"tempo-inatividade") == 0){
				/*
				tempo_inatividade = atoi(comando[1]);
				strcat(msg, "Tempo de intavidade alterado para: ");
				strcat(msg, comando[1]);
				strcat(msg, "!\n");
				write(fd_out, msg, strlen(msg));
				*/
			}
			else if(strcmp(comando[0],"ajuda") == 0){
				printHelp(fd_out);
			}
			else if(strcmp(comando[0],"listar") == 0){
				if (listarTarefas(indice_tarefa, fd_out,EM_EXECUCAO) == 2) write(fd_out, "Nao existem tarefas em execucao!\n",34);
			}
			else if(strcmp(comando[0],"historico") == 0){
				if (listarTarefas(indice_tarefa, fd_out,TERMINADAS) == 2) write(fd_out, "Nao existem tarefas terminadas!\n",32);
			}
			else if(strcmp(comando[0],"executar") == 0){
				if(comando[1] == NULL)write(fd_out, "Falta informacao!\n", 18);
				else{
					strcat(msg, "Nova tarefa #");
					itoa(indice_tarefa, aux);
					strcat(msg, aux);
					strcat(msg, "\n");
					write(fd_out, msg, strlen(msg));

					removeApice(comando[1]);

					if(fork()==0){
						executar(comando[1], tempo_execucao, indice_tarefa++);
						_exit(0);
					}
					indice_tarefa++;
				}
			}
			else if(strcmp(comando[0],"terminar") == 0){
				if(comando[1] == NULL)write(fd_out, "Falta informacao!\n", 18);
				else{
					switch(terminar(atoi(comando[1]), indice_tarefa)){
						case 0: 
							write(fd_out, "Tarefa terminada com sucesso!\n", 30);
							break;
						case 1: 
							write(fd_out, "Indicie invalido!\n",18);
							break;
						case 2: 
							write(fd_out, "Erro ao abrir o ficheiro pids.txt!\n",35);
							break;
						case 3: 
							write(fd_out, "Erro ao abrir o ficheiro log.txt!\n",34);
							break;
						case 4:
							write(fd_out, "A tarefa nao esta em execucao!\n", 31);
							break;
						default:
							break;	
				}
				}
			}else if(strcmp(comando[0],"output") == 0){
				if(comando[1] == NULL)write(fd_out, "Falta informacao!\n", 18);
				else{
					
				}
			}
			else{
				write(fd_out,"Comando invalido!\n",18);
			}

		}
	}
	close(fd_log);
	close(fd_out);
	close(fd_in);
	close(fd_pids);

	return 0;
}
