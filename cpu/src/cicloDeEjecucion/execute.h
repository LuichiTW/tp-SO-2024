#ifndef EXECUTE
#define EXECUTE

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
#include "funcionesexecute.h"


extern t_log *loggerPrincipal;

void funExecute(struct s_instruccion*);
int decodificarRegistro(char[]);

#endif // !EXECUTE