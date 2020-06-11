#include "auxiliares.h"

char *strtok(char *str, const char *delim);

static char** split(char* command){

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
