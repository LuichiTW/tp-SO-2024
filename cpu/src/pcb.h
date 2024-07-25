#ifndef PCB_H
#define PCB_H

typedef struct
{
    int pid;     // Identificador del proceso
    int pc;      // Contador de Programa
    int quantum; // Unidad de tiempo utilizada para el algoritmo VRR
    
} t_pcb;

extern t_pcb pcb;

t_pcb recibir_pcb(int socket_cliente_kernel_dispatch, int socket_cliente_kernel_interrupt);

#endif // PCB_H