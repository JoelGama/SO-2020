#include "auxiliares.h"

/** 
 * *pids    - Array com os pids da tarefa em execução
 * pidCount - Número de pids guardado no array anterior
 * log_pos  - Posição do estado da tarefa atual no ficheiro de logs
 **/
int *pids;
int pidCount;
off_t log_pos;

/** 
 * Função responsável por estabelecer um
 * comportamento para o sinal SIGALRM
 **/
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
    close(fd_log);

    kill(getpid(),SIGKILL);
}

/** 
 * Função auxiliar responsável por separar uma 
 * string segundo um caracter dado como argumento
 **/
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

/** 
 * Função auxiliar responsável por separar uma 
 * string comando conhecendo as posições pré 
 * definidas dos parâmetros 
 **/
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

/** 
 * Função responsável por imprimir a ajuda 
 * para o cliente
 **/
int printHelp(int fildes){
	int i = 0, bytes = 0;
	char *buf[1024];

	int fd = open("help.txt",O_RDONLY, 0600);

	while ((i = read(fd,buf,1024)) >0){
        bytes += i;
    }

	close(fd);

	write(fildes,buf,bytes);

	return 0;
}

/** 
 * Função auxiliar responsável por remover
 * new lines ('\n') de uma string
 **/
void removeNewLine(char *string){
    int i = 0;
    while(string[i] > 30) i++;
    string[i] = '\0';
}

/** 
 * Função responsável por executar as tarefas
 * passadas como argumento
 **/
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
    char pid_buf[12] = "";

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

        itoa(pid, pid_buf);
        strcat(pid_buf, " ");
        write(fd_pids, pid_buf, strlen(pid_buf));

        next++;
        index++;
        j+=2;
    }
    pidCount = 0;
    
    itoa(getpid(), pid_buf);
    strcat(pid_buf, "\n");
    write(fd_pids, pid_buf, strlen(pid_buf));
    
    for(i = 0; i < count + 1; i++)
        wait(&status);
    
    int seek = -strlen(buf) + strlen(id);
    lseek(fd_log, seek , SEEK_CUR);
    write(fd_log, "1 ", 2);
    
    for(i = 0; i < 2 * count; i++){
        close(fd[i]);
    }
    
    free(cmds);
    free(argv);
    free(pids);

    close(fd_log);
    close(fd_pids);

    return 0;
}

/** 
 * Função auxiliar responsável por remover os 
 * caracteres "''" (ápice) de uma string
 **/
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

/** 
 * Função auxiliar responsável por converter
 * um interiro para alfanumérico
 **/
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

/** 
 * Função auxiliar utilizada na função itoa
 **/
void reverse(char s[]){
     int i, j;
     char c;

     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
}

/** 
 * Função responsável por terminar uma dada tarefa
 **/
int terminar(int indice_tarefa, int last_indice_tarefa){
    int fd_pids;
    if(last_indice_tarefa <= indice_tarefa){
        return(1);
    }

    if((fd_pids = open("pids.txt", O_RDWR) ) < 0){
		exit(2);
    }

    char buffer[100];
    off_t bytes = 0;

    int i = 0;

    while(i <= indice_tarefa){
        bytes += readln(fd_pids, buffer, 100);
        i++;
    }

    removeNewLine(buffer);
    char *c;
    int pid_to_kill;
    c = strtok(buffer, " ");
    int indice_offset = strlen(c) + 1;

    while ((c = strtok(NULL, " ")) != NULL){
        pid_to_kill = atoi(c);
        kill(pid_to_kill, SIGKILL);
    }

    int fd_log;
    if((fd_log = open("log.txt", O_RDWR) ) < 0){
		exit(3);
    }

    bytes = 0;
    i = 0;

    while(i < indice_tarefa){
        bytes += readln(fd_log, buffer, 100);
        i++;
    }

    lseek(fd_log, indice_offset, SEEK_CUR);
    char status[2];
    read(fd_log, status, 1);

    if(atoi(status) != 0){
        return(4);
    }

    lseek(fd_log, -1, SEEK_CUR);
    write(fd_log, "4", 1);

    close(fd_pids);
    close(fd_log);

    return 0;
} 

/** 
 * Função responsável por listar tarefas em 
 * execução ou terminadas (decidido segundo 
 * um argumento passado)
 **/
int listarTarefas(int indice_tarefa, int fildes, int arg){

    char *buf = NULL;
    char atual[100];
    char *toWrite = NULL;
    int fd_log;
    int bytes = 0;
    int i = 0;
    int nProcessos = 0;

    buf = malloc(sizeof(char) * 1024);

    if((fd_log = open("log.txt", O_RDONLY) ) < 0){
        perror("open log");
		exit(1);
    }

    while((bytes = readln(fd_log,atual,100)) > 0 && i++ < indice_tarefa){
        
        toWrite = malloc(sizeof(char) * 100);
            
        char* s1 = strtok(atual," ");
        char* s2 = strtok(NULL," ");
        char* s3 = strtok(NULL,"\n");

        int x = atoi(s2);

        if (arg == 0){
            if(x == 0){
                strcat(toWrite,"#");
                strcat(toWrite,s1);
                strcat(toWrite," em execução: ");

                strcat(toWrite,s3);

                strcat(toWrite,"\n");
                strcat(buf,toWrite);
                nProcessos++;
            }
        } else {
            if(x != 0){
                strcat(toWrite,"#");
                strcat(toWrite,s1);
                switch(x){
                    case 1:
                        strcat(toWrite," terminou: ");
                        break;
                    case 2:
                        strcat(toWrite," max execução: ");
                        break;
                    case 3:
                        strcat(toWrite," max inatividade: ");
                        break;
                    case 4:
                        strcat(toWrite," morto pelo utilizador: ");
                        break;
                    default:
                        break;
                }
                strcat(toWrite,s3);
                strcat(toWrite,"\n");
                strcat(buf,toWrite);
                nProcessos ++;
            }
        }
        free(toWrite);
    }
    if(nProcessos > 0) write(fildes,buf,strlen(buf));
    else return 2;

    free(buf);
    close(fd_log);
        
    return 0;
}

/** 
 * Função auxiliar responsável por ler uma 
 * linha de um ficheiro
 **/
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