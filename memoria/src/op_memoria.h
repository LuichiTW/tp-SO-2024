#ifndef OP_MEMORIA_H
#define OP_MEMORIA_H

#include <stdlib.h>
#include <stdio.h>
#include <commons/log.h>
#include <commons/error.h>
#include <commons/collections/list.h>
#include <string.h>

#include <readline/readline.h>

#include "memoria.h"

void enviar_instruccion(int, uint);
void crear_proceso(int, const char*);
void finalizar_proceso(int);
char **leer_script(const char*);
void resize_proceso(int, uint);

int iceildiv(int, int);

#endif