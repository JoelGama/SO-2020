#include "auxiliares.h"

int executar(char *command) {

    int status;
    int i = 0, j = 0;
    int count = 0, next = 1, index = 0;
    pid_t pid;

    char** cmds = malloc(sizeof(char*) * 50);
    char** argv = malloc(sizeof(char*) * 10);
    
    cmds = split(command,"|");

    for (i = 0; cmds[i] != NULL; i++){
        count++;
    }
    
    int fd[2*count];

    for(i = 0; i < count; i++){
        if(pipe(fd + i*2) < 0) {
            perror("couldn't pipe");
            exit(EXIT_FAILURE);
        }
    }

    j = 0, i = 0;
    while(cmds[index] != NULL) {
        if((pid = fork()) == 0) {

            printf("Next: %d\n",next);
            //if not last command
            if(cmds[next] != NULL){
                if(dup2(fd[j + 1], 1) < 0){
                    perror("dup2 Last comand");
                    exit(EXIT_FAILURE);
                }
            }
            //if not first command
            if(index != 0){
                if(dup2(fd[j-2],0) < 0){
                    perror("dup2 not 1st comand");
                    exit(EXIT_FAILURE);

                }
            }

            for(i = 0; i < 2 * count; i++){
                close(fd[i]);
            }

            printf("Split do comando\n");
            argv = split(cmds[index]," ");

            execvp(argv[0], argv);
        }
        next++;
        index++;
        j+=2;
    }
 
    /* Parent closes the pipes and wait for children */
    for(i = 0; i < 2 * count; i++){
        close(fd[i]);
    }

    for(i = 0; i < count + 1; i++)
        wait(&status);

    free(cmds);
    free(argv);

    return 0;
}

int main(int argc, char const *argv[]){
    
    char* cmd = malloc(sizeof(char*) * 100);

    for(int i = 1; i < argc; i++){
        strcat(cmd,argv[i]);
        strcat(cmd," ");
    }

    int i = executar(cmd);

    free(cmd);

    return i;
}