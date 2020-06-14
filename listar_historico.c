#include "auxiliares.h"

void listar(int indice_tarefa){

    char buf[1024];
    char *atual;
    char *toWrite;
    int n = 0, j;

    char** ind = malloc(sizeof(char*) * 50);

    int fd_log;
    if((fd_log = open("log.txt", O_RDWR) ) < 0){
        perror("open log");
		exit(1);
    }

    for(int i = 0; i < indice_tarefa; i++){
        n = read(fd_log,atual,100);
        ind = split(atual," ");
        if(ind[1] == "0"){
            strcat(toWrite,"#");
            strcat(toWrite,ind[0]);
            strcat(toWrite," em execução ");
            j = 2;
            while(ind[j] != NULL){
                strcat(toWrite,ind[j]);
                strcat(toWrite," ");
            }
            strcat(toWrite,"\n");
            write(buf,toWrite,strlen(toWrite));
        }
    }
    free(ind);
    write(1,buf,strlen(buf));
}

void historico(int indice_tarefa){

    char buf[1024];
    char *atual;
    char *toWrite;
    int n = 0, j;

    char** ind = malloc(sizeof(char*) * 50);

    int fd_log;
    if((fd_log = open("log.txt", O_RDWR) ) < 0){
        perror("open log");
		exit(1);
    }

    for(int i = 0; i < indice_tarefa; i++){
        n = read(fd_log,atual,100);
        ind = split(atual," ");

        int x = atoi(ind[1]);

        switch(x){
            case(1):
                strcat(toWrite,"#");
                strcat(toWrite,ind[0]);
                strcat(toWrite," terminado ");
                j = 2;
                while(ind[j] != NULL){
                    strcat(toWrite,ind[j]);
                    strcat(toWrite," ");
                }
                break;
            case(2):
                strcat(toWrite,"#");
                strcat(toWrite,ind[0]);
                strcat(toWrite," tempo de execução ");
                j = 2;
                while(ind[j] != NULL){
                    strcat(toWrite,ind[j]);
                    strcat(toWrite," ");
                }
                break;

            case(3):
                strcat(toWrite,"#");
                strcat(toWrite,ind[0]);
                strcat(toWrite," tempo de inatividade ");
                j = 2;
                while(ind[j] != NULL){
                    strcat(toWrite,ind[j]);
                    strcat(toWrite," ");
                }
                break;

            case(4):
                strcat(toWrite,"#");
                strcat(toWrite,ind[0]);
                strcat(toWrite," terminado pelo utilizador ");
                j = 2;
                while(ind[j] != NULL){
                    strcat(toWrite,ind[j]);
                    strcat(toWrite," ");
                }
                break;

            default:
                break;

            strcat(write,"\n");
            write(buf,toWrite,strlen(toWrite));
        }
        
    }
    free(ind);
    write(1,buf,strlen(buf));
}

ssize_t readln(int fildes, char* line, size_t size){
	ssize_t n = read(fildes,line,size);
    int i;

	if(n > 0){
		for(i = 0; i < n && line[i] != '\n'; i++);
		lseek(fildes,-(n - i - 1), SEEK_CUR);
        i++;
	}	
	return i;
}