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
int crear_proceso(int, const char*);
void finalizar_proceso(int);
char **leer_script(const char*);
void acceso_tabla_paginas(int, int);
void resize_proceso(int, uint);

void leer_memoria(int, size_t);
void escribir_memoria(int, size_t, void*);

int iceildiv(int, int);

#endif