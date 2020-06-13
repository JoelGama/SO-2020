#include "auxiliares.h"

char *strtok(char *str, const char *delim);

char** split(char* command, char* s){

    int i = 0;
    char** argv = malloc(sizeof(char*) * 100);

    char* c = strtok(command, " ");
    while(c){
        argv[i++] = strdup(c);
        c = strtok(NULL," ");  
    }

    argv[i] = NULL;

    return argv;
}

int printHelp(){
	int i = 0, bytes = 0;
	char *buf[1024];

	int fd = open("help.txt",O_RDONLY, 0600);

	while ((i = read(fd,buf,1024)) >0){
        bytes += i;
    }

	close(fd);

	write(1,buf,bytes);

	return 0;
}
