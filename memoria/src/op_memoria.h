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

char *enviar_instruccion(int, uint);
int crear_proceso(int, const char*);
void finalizar_proceso(int);
char **leer_script(const char*);
int acceso_tabla_paginas(int, int);
int resize_proceso(int, uint);

char *leer_memoria(int, int);
void escribir_memoria(int, int, void*);

int iceildiv(int, int);

#endif