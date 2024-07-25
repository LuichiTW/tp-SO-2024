#ifndef MMU_H
#define MMU_H

#include <stdlib.h>
#include <math.h>
#include <commons/collections/list.h>

#include "conexiones.h"
#include "cicloDeEjecucion/checkInterrupt.h"

typedef struct {
    int pagina;
    int desplazamiento;
} t_dir_logica;

t_dir_logica separar_dir_logica(int dir_logica);
int obtener_direccion_fisica(t_dir_logica dir_logica);
int obtener_frame(int pagina);

#endif