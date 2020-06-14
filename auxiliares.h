#ifndef AUXILIARES_H
#define AUXILIARES_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

char** split(char* command, char* s);

char** splitComando(char* commando);

int printHelp();

int executar(char *command,int tempo_execucao, int indice_tarefa);

void removeNewLine(char *string);

void removeApice(char *string);

void itoa(int n, char s[]);

void reverse(char s[]);

ssize_t readln(int fildes, char* line, size_t size);

int terminar(int indice_tarefa, int last_indice_tarefa);

int listar();

int historico(int indice_tarefa);

#endif