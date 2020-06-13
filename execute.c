#include "auxiliares.h"

int main(int argc, char const *argv[]){
    
    char* cmd = malloc(sizeof(char*) * 100);

    for(int i = 1; i < argc; i++){
        strcat(cmd,argv[i]);
        strcat(cmd," ");
    }

    int i = executar(cmd,30);

    free(cmd);

    return i;
}