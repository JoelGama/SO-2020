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

int printHelp();
#endif