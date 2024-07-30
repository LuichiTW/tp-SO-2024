#ifndef PCB_H
#define PCB_H

#include "registros.h"

typedef struct
{
    int pid;     // Identificador del proceso
    int pc;      // Contador de Programa
    int quantum; // Unidad de tiempo utilizada para el algoritmo VRR
    t_registros_uso_general registros; // Estado de los registros de uso general
} t_pcb;

extern t_pcb pcb;

void recibir_pcb(int socket_cliente_kernel_dispatch, int socket_cliente_kernel_interrupt);

void actualizar_pcb();

#endif // PCB_H