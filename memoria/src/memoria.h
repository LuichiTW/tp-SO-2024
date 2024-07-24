#ifndef MEMORIA_H
#define MEMORIA_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/error.h>
#include <commons/bitarray.h>
#include <commons/string.h>
#include <commons/collections/list.h>

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
    int cant;
} t_tabla_paginas;

typedef struct {
    //int pagina;
    int frame;
} t_pagina;

typedef struct {
    int pid;
    char **instrucciones;
    int cant_instrucciones;
} t_proceso;

enum OPERACIONES_MEMORIA {
    CREAR_PROCESO,
    ENVIAR_INSTRUCCION,
    FINALIZAR_PROCESO,
    ACCESO_TABLA_PAGINAS,
    LEER_MEMORIA,
    ESCRIBIR_MEMORIA,
    RESIZE_PROCESO
};

extern void *mem_usuario;
extern t_bitarray *frames_ocupados;
extern t_config_memoria config_memoria;
extern t_list *procesos_actuales;
extern t_list *tablas_paginas;
extern u_int cant_frames;

void cargar_config();
void iniciar_mem_usuario();
void iniciar_bitmap_frames_ocupados();
t_tabla_paginas *crear_tabla_paginas(int);

u_int cant_frames_libres();
// Retorna el índice del primer frame libre en el bitarray, o -1 si no hay.
int prox_frame_libre();
void print_frames_ocupados(int);

t_tabla_paginas *obtener_tabla_por_pid(int);
t_proceso *obtener_proceso_por_pid(int);

t_log *crear_memlogger();
t_log *alt_memlogger();

#endif // MEMORIA_H
