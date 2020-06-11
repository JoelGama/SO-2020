#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

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

int executar(char* command) {

    int status;
    int i = 1;
    int j = 0;
    pid_t pid;
    int cc = 0;
    int count = 0;
    char *cmds[20];
    char *cmd[20];

    char** argv = malloc(sizeof(char*) * 100);

    char* str;
    char* a;
    char aux[50];
    strcpy(aux,command);

    for(i = 0; aux[i] != '\0'; i++) {

        if (aux[i+1] == '\0'){
            a = aux[i];
            strcat(str,a);
            cmds[j++] = str;
            printf("%s\n",str);
            break;
        }

        if (aux[i] == '|'){
            cmds[j++] = str;
            printf("%s\n",str);
            str = "";
        } else {

        a = strcpy(a,aux[i]);
        trcat(str,a);
        }
    }
    i-1;
    cmds[i] = NULL;
    cc = i;

    int pipefds[2*cc];

    for(i = 0; i < cc; i++){
        if(pipe(pipefds + i*2) < 0) {
            perror("couldn't pipe");
            exit(EXIT_FAILURE);
        }
    }

    j = 0;
    while(cmds[count] != NULL) {
        pid = fork();
        if(pid == 0) {

            //if not last command
            if(cmds[count+1] != NULL){
                if(dup2(pipefds[j + 1], 1) < 0){
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }
            count--;
            //if not first command&& j!= 2*numPipes
            if(count != 0 ){
                if(dup2(pipefds[j-2], 0) < 0){
                    perror(" dup2");///j-2 0 j+1 1
                    exit(EXIT_FAILURE);

                }
            }

            for(i = 0; i < 2*cc; i++){
                close(pipefds[i]);
            }

            argv = split(cmds[count]);

            printf("argv[%d]: %s\n",count,(char*)argv[0]);

            if(execvp(argv[0], argv) < 0){
                    perror(argv[0]);
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
    for(i = 0; i < 2 * cc; i++){
        close(pipefds[i]);
    }

    for(i = 0; i < cc + 1; i++)
        wait(&status);

    return 0;
}


int main(int argc, char const *argv[]){
    
    int i = executar((char*)argv[1]);

    return i;
}