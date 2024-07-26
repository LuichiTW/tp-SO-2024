#ifndef MMU_H
#define MMU_H

#include <stdlib.h>
#include <math.h>
#include <commons/collections/list.h>

#include "conexiones.h"
#include "tlb.h"
//#include "cicloDeEjecucion/checkInterrupt.h"

typedef struct {
    int pagina;
    int desplazamiento;
} t_dir_logica;

typedef struct {
    int cant_paginas;
    int *frames;
    int *bytes_por_pag;
} t_lista_dir_fisicas;

//int obtener_direccion_fisica(t_dir_logica dir_logica);

t_dir_logica separar_dir_logica(int dir_logica);

t_lista_dir_fisicas obtener_direcciones_fisicas(int dir_logica_i, int tam_dato);

// Retorna cantidad de páginas.
int fraccionar_direccion(t_dir_logica dir_logica, int tam_dato);

int obtener_frame(int pagina);

int pedir_frame_memoria(int pagina);

#endif