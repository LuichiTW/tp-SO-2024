#ifndef CPU_PCB_H
#define CPU_PCB_H

#include <utils/pcb.h>
#include "registros.h"

extern t_pcb pcb;

void recibir_pcb(int socket_cliente_kernel_dispatch, int socket_cliente_kernel_interrupt);

void actualizar_pcb();

#endif // CPU_PCB_H