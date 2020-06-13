#include "auxiliares.h"

char *strtok(char *str, const char *delim);

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

char** splitComando(char* commando){

    int i = 0;
    char** argv = malloc(sizeof(char*) * 100);

    char* c = strtok(commando," ");
    removeNewLine(c);
    argv[i++] = strdup(c);
    if((c = strtok(NULL, "\0"))!= NULL){
        removeNewLine(c);
        argv[i++] = strdup(c);
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

void removeNewLine(char *string){
    int i = 0;
    while(string[i] > 30) i++;
    string[i] = '\0';
}

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

            printf("Let's executar\n");
            execvp(argv[0], argv);

            printf("Executei!!!!\n");

        }
        next++;
        index++;
        j+=2;

        printf("Let's executar\n");
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

void removeApice(char *string){
    int i = 0;
    int j = 0;

    while(string[i] != '\0'){
        if(string[i] == 39){
            i++;
        }
        else{
            string[j] = string[i];
            j++;
            i++;
        }
    }

    string[j] = '\0';
}