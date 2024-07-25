#include "pcb.h"

t_pcb pcb;

t_pcb recibir_pcb(int socket_cliente_kernel_dispatch, int socket_cliente_kernel_interrupt){
    t_pcb nuevo_pcb;
    nuevo_pcb.pid = 1;
    nuevo_pcb.pc = 0;
    nuevo_pcb.quantum = 2;

    return nuevo_pcb;

    // Recibir PCB
    /* t_pcb *pcb = malloc(sizeof(t_pcb));
    int bytesRecibidos = recv(socket_cliente_kernel_dispatch, pcb, sizeof(t_pcb), 0);
    if (bytesRecibidos <= 0) {
        log_error(loggerPrincipal, "No se pudo recibir el PCB");
        exit(EXIT_FAILURE);
    }
    ////log_info(loggerPrincipal, "Se recibio el PCB correctamente");
    return pcb; */
}