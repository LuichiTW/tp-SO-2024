#ifndef OP_MEMORIA_H
#define OP_MEMORIA_H

#include <stdlib.h>
#include <stdio.h>
#include <commons/log.h>
#include <commons/error.h>
#include <commons/collections/list.h>
#include <string.h>

#include "memoria.h"

void enviar_instruccion(int);
void crear_proceso(int);
char **leer_script(const char*);
void resize_proceso(int, size_t);

#endif