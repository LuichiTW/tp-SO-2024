#ifndef CPU_PCB_H
#define CPU_PCB_H

#include <utils/pcb.h>
#include "conexiones.h"
#include "registros.h"

extern t_pcb pcb;
extern bool proceso_ejecutando;

void recibir_pcb();
void actualizar_pcb();

#endif // CPU_PCB_H