#ifndef MAIN_MEMORIA
#define MAIN_MEMORIA

#include <stdlib.h>
#include <stdio.h>
#include <utils/conexiones/conexiones_servidor.h>
#include <utils/conexiones/conexiones_cliente.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/error.h>
#include <commons/memory.h>
#include <readline/readline.h>

#include "memoria.h"
#include "op_memoria.h"

void recibir_conexiones(int);
void recibir_solicitudes(int);

#endif // MAIN_MEMORIA