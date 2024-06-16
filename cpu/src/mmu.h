#ifndef MMU_H
#define MMU_H

#include <stdlib.h>
#include <math.h>
#include <commons/collections/list.h>

#include "conexiones.h"
#include "cicloDeEjecucion/checkInterrupt.h"

typedef struct {
    uint pagina;
    uint desplazamiento;
} t_dir_logica;

extern uint tam_pagina;

t_dir_logica separar_dir_logica(uint dir_logica);
uint obtener_direccion_fisica(t_dir_logica dir_logica);
int obtener_frame(int pagina);

#endif