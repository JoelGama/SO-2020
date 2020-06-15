
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "auxiliares.h"

/*
###### Tams.txt ########

12 46 99 123 231 345 4312

cada valor é a posição onde 
termina de ser escrito o seu output
*/

int output(int filedes_Out,int filedes_output,int indice_tarefa,int indice_tarefa_max){
    if(indice_tarefa >= indice_tarefa_max){
        return 1;
    }

    char buffer[1024];
    int read_bytes;
    int fd_tams;
    if((fd_tams = open("tams.txt",O_RDONLY,0200))<0){
        perror("open tams.txt");
        return 2;
    }

    if((read_bytes = read(fd_tams,buffer,1024))<=0){
        return 3;
    }

    int pos[2];
    char* aux = NULL;
    
    if(indice_tarefa == 0){
        pos[0] = 0;
        aux = strtok(buffer," ");
        pos[1] = atoi(aux);
    }else{
        int i=1;
        aux = strtok(buffer," ");
        while(i<indice_tarefa){
            aux = strtok(NULL," ");
        }
        pos[0] = atoi(aux);
        aux = strtok(NULL," ");
        pos[1] = atoi(aux);
    }
    int leitura = pos[1] - pos[0];
    lseek(filedes_Out,pos[0],SEEK_SET);
    buffer[0] = '\0';
    if((read_bytes=read(filedes_output,buffer,leitura)>0)){
        write(filedes_Out,buffer,read_bytes);
    }

    return 0;
}