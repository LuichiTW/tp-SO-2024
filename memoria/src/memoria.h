#ifndef MEMORIA_H
#define MEMORIA_H

#include <stdlib.h>
#include <stdio.h>
#include <commons/config.h>
#include <string.h>
#include <math.h>
#include <commons/log.h>
#include <commons/error.h>
#include <commons/bitarray.h>
#include <commons/string.h>
#include <pthread.h>

typedef struct {
    int puerto_escucha;
    int tam_memoria;
    int tam_pagina;
    char* path_instrucciones;
    int retardo_respuesta;
    t_config *config;
} t_config_memoria;

typedef struct {
    int pid;
    t_list *paginas;
} t_tabla_paginas;

typedef struct {
    int pagina;
    int marco;
} t_pagina;

typedef struct {
    int pid;
    char **instrucciones;
    int cant_instrucciones;
} t_proceso;

enum OPERACIONES_MEMORIA {
    CREAR_PROCESO,
    ENVIAR_INSTRUCCION
};

extern void *mem_usuario;
extern t_bitarray *frames_ocupados;
extern t_config_memoria config_memoria;
extern t_list *procesos_actuales;
extern t_list *tablas_paginas;

void cargar_config();
void iniciar_mem_usuario();
void iniciar_bitmap_frames_ocupados();
t_tabla_paginas *crear_tabla_paginas(int);
t_log *crear_memlogger();
t_log *alt_memlogger();

#endif // MEMORIA_H
