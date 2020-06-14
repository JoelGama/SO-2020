#include "auxiliares.h"

int *pids;
int pidCount;
off_t log_pos;

void timeout_handler(int signum){
    for(int i=0; i<pidCount; i++){
        kill(pids[i],SIGKILL);
    }
    int fd_log;
    if((fd_log = open("log.txt", O_RDWR) ) < 0){
        perror("open log");
		exit(1);
    }

    printf("%ld \n", lseek(fd_log, log_pos, SEEK_SET));
    write(fd_log, "2 ", 2);

    kill(getpid(),SIGKILL);
}

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

int executar(char *command,int tempo_execucao, int indice_tarefa) {
    if((signal(SIGALRM,timeout_handler)) < 0){
        perror("SIGALRM Handler error");
    }

    int fd_log, fd_pids;
    if((fd_log = open("log.txt", O_RDWR) ) < 0){
        perror("open log");
		exit(1);
    }

    if((fd_pids = open("pids.txt", O_RDWR) ) < 0){
        perror("open pids");
		exit(1);
    }


    char id[10];
    char buf[100];

    itoa(indice_tarefa, id);

    strcat(buf, id);
    strcat(buf, " 0 ");
    strcat(buf, command);
    strcat(buf, "\n");

    log_pos = lseek(fd_log, 0, SEEK_END);
    write(fd_log, buf, strlen(buf));
    log_pos += strlen(id) + 1;

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

    pidCount=count;
    pids = malloc(sizeof(int)*pidCount);
    if(tempo_execucao>0){
        alarm(tempo_execucao);
    }

    strcat(id, " ");
    lseek(fd_pids, 0, SEEK_END);
    write(fd_pids, id, strlen(id));

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

            argv = split(cmds[index]," ");

            execvp(argv[0], argv);
        }
        pids[index] = pid;

        char pid_buf[10] = "";
        itoa(pid, pid_buf);
        strcat(pid_buf, " ");
        write(fd_pids, pid_buf, strlen(pid_buf));

        next++;
        index++;
        j+=2;
    }
    pidCount = 0;
    write(fd_pids, "\n", 1);
    
    for(i = 0; i < count + 1; i++)
        wait(&status);
    
    int seek = -strlen(buf) + strlen(id);
    lseek(fd_log, seek , SEEK_CUR);
    write(fd_log, "1 ", 2);

    printf("Acabou\n");
    
    for(i = 0; i < 2 * count; i++){
        close(fd[i]);
    }
    
    free(cmds);
    free(argv);
    free(pids);

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

void itoa(int n, char s[]){
     int i, sign;

     if ((sign = n) < 0)
         n = -n;
     i = 0;
     do {
         s[i++] = n % 10 + '0';
     } while ((n /= 10) > 0);
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
}  

void reverse(char s[]){
     int i, j;
     char c;

     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
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

