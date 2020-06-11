#include "auxiliares.h"

char** split(char* command, char* s){

    int i = 0;
    char** argv = malloc(sizeof(char*) * 100);

    char* c = strtok(command,s);
    while(c){
        argv[i++] = strdup(c);
        c = strtok(NULL,s);  
    }

    argv[i] = NULL;

    return argv;
}

int executar(char *command) {

    int status;
    int i = 1;
    int j = 0;
    pid_t pid;

    char** cmds = malloc(sizeof(char*) * 50);
    char** argv = malloc(sizeof(char*) * 10);

    printf("Comando input: %s\n",command);

    int fd[2];

    if(pipe(fd) < 0) {
        perror("couldn't pipe");
        exit(EXIT_FAILURE);
    }

    cmds = split(command,"|");

    j = 0;
    if((pid = fork()) == 0) {

        dup2(fd[0],0);

        close(fd[0]);
        close(fd[1]);

        argv = split(command," ");

        if(execvp(argv[0], argv) < 0){
            perror(argv[0]);
            exit(EXIT_FAILURE);
        }

    } else 
        if(pid < 0){
            perror("error");
            exit(EXIT_FAILURE);
    }
 
    for(i = 0; i < 2; i++){
        close(fd[i]);
    }

    wait(&status);

    return 0;
}

int main(int argc, char const *argv[]){
    
    char* cmd = malloc(sizeof(char*) * 100);

    for(int i = 1; i < argc; i++){
        strcat(cmd,argv[i]);
        strcat(cmd," ");
    }

    int i = executar(cmd);

    return i;
}