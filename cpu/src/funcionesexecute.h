#ifndef FUNEXECUTE
#define FUNEXECUTE

#include <stdlib.h>
#include <stdio.h>
#include <utils/conexiones/conexiones_servidor.h>
#include <utils/conexiones/conexiones_cliente.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/error.h>
#include <readline/readline.h>

#include "registros.h"

// #include "main.h"
extern t_log *loggerPrincipal;

//suma dos registros y coloca el resultado en el primer registro
void fSUM(lista_registros_CPU, lista_registros_CPU);

//resta dos registros y coloca el resultado en el primer registro
void fSUB(lista_registros_CPU, lista_registros_CPU);

//Establece el valor pasado en el registro indicado
void fSET(enum lista_registros_CPU, int);

void fJNZ(enum lista_registros_CPU, uint32_t);
#endif // FUNEXECUTE