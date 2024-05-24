#ifndef MEMORIA_H
#define MEMORIA_H

#include <stdlib.h>
#include <stdio.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/error.h>
#include <commons/bitarray.h>

typedef struct {
    int puerto_escucha;
    int tam_memoria;
    int tam_pagina;
    char* path_instrucciones;
    int retardo_respuesta;
} t_config_memoria;

extern void *mem_usuario;
extern t_config_memoria config_memoria;

void cargar_config();
void iniciar_mem_usuario();
t_log *crear_memlogger();

#endif // MEMORIA_H
