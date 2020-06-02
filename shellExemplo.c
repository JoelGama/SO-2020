#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

void my_prompt(){
	int static first_call = 1;
	if (first_call){
		const char* clear_Screen = "\e[1;1H\e[2J";
		write(1,clear_Screen,12);
		first_call = 0;
	}

	write(1,"argus$",6);
}

int main(int argc, char *argv[]){

	char cmd[100], command[100], *parameters[20];

	while(1){
		my_prompt();
		// Ler input do usuario
		// Enviar o commando para o server
	}

	return 0;
}

