#include <string.h>
#include <unistd.h>

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

void execturar(char* command) {

    int status;
    int i = 0;
    pid_t pid;
    int cmds[20]
    int cmd[10]

    char** argv = malloc(sizeof(char*) * 100);

    int pipefds[2*numPipes];

    for(i = 0; i < (numPipes); i++){
        if(pipe(pipefds + i*2) < 0) {
            perror("couldn't pipe");
            exit(EXIT_FAILURE);
        }
    }

    char* token = strtok(command, "|");
    i = 0
    while( token != NULL ) {
        cmds[i] = token;
        i++;
        token = strtok(NULL, "|");
    }
    cmds[i] = -1

    count = 0
    int j = 0;
    while(cmds[count] != -1) {
        pid = fork();
        if(pid == 0) {

            //if not last command
            if(cmds[count+1] != -1){
                if(dup2(pipefds[j + 1], 1) < 0){
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }

            //if not first command&& j!= 2*numPipes
            if(count != 0 ){
                if(dup2(pipefds[j-2], 0) < 0){
                    perror(" dup2");///j-2 0 j+1 1
                    exit(EXIT_FAILURE);

                }
            }

            for(i = 0; i < 2*numPipes; i++){
                    close(pipefds[i]);
            }

            argv = split(cmds[count]);
            if( execvp(argv[0], argv) < 0 ){
                    perror(*command->arguments);
                    exit(EXIT_FAILURE);
            }

        } else if(pid < 0){
            perror("error");
            exit(EXIT_FAILURE);
        }

        count++;
        j+=2;
    }

    /**Parent closes the pipes and wait for children*/
    for(i = 0; i < 2 * numPipes; i++){
        close(pipefds[i]);
    }

    for(i = 0; i < numPipes + 1; i++)
        wait(&status);
}