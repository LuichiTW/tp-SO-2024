#include "pcb.h"

t_pcb pcb;

void recibir_pcb(int socket_cliente_kernel_dispatch, int socket_cliente_kernel_interrupt){
    // ! Reemplazar por los que vengan de kernel
    pcb.pid = 1;
    pcb.pc = 0;
    pcb.quantum = 2;
    pcb.registros;
    // !

    regcpu = reg_gen_to_reg_cpu(pcb.registros);
    regcpu.PC = pcb.pc;

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

void actualizar_pcb() {
    pcb.pc = regcpu.PC;
    pcb.registros = reg_cpu_to_reg_gen(regcpu);
}