#ifndef FETCH
#define FETCH

#include "registros.h"

#include <stdlib.h>
#include <stdio.h>
#include <utils/conexiones/conexiones_servidor.h>
#include <utils/conexiones/conexiones_cliente.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/error.h>
#include <readline/readline.h>
#include <commons/string.h>


extern t_log *loggerPrincipal;
extern registros_cpu regcpu;

char *funFetch(int);


#endif // !FETCH