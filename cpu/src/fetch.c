#include "fetch.h"

char *funFetch(int socketMemoria){
    //!PRUEBA DE FUNCIONAMIENTO
    char *instruccion = readline("> ");
    regcpu.PC++;
    return instruccion;
}