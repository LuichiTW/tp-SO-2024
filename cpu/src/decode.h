#ifndef DECODE
#define DECODE

#include <stdlib.h>
#include <stdio.h>
#include <utils/conexiones/conexiones_servidor.h>
#include <utils/conexiones/conexiones_cliente.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/error.h>
#include <readline/readline.h>
#include <commons/string.h>

#include "listaInstrucciones.h"


void *funDecode(char[]);
int decodificarInstruccion(char[]);

extern t_log *loggerPrincipal;

#endif // !DECODE