#include "auxiliares.h"

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
 * Função auxiliar responsável por remover
 * new lines ('\n') de uma string
 **/
void removeNewLine(char *string){
    int i = 0;
    while(string[i] > 30) i++;
    string[i] = '\0';
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