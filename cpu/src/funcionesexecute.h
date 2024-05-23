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

#include "main.h"
extern t_log *loggerPrincipal;

//suma dos registros y coloca el resultado en el primer registro
void SUM(lista_registros_CPU, lista_registros_CPU);

void SUB(lista_registros_CPU, lista_registros_CPU);

void SET(lista_registros_CPU, int);

#endif // FUNEXECUTE