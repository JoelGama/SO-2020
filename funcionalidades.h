#ifndef FUNCIONALIDADES_H
#define FUNCIONALIDADES_H

#include "auxiliares.h"

int printHelp(int fildes);

int executar(char *command,int tempo_execucao, int indice_tarefa);

int terminar(int indice_tarefa, int last_indice_tarefa);

int listarTarefas(int indice_tarefa, int fildes,int arg);

#endif