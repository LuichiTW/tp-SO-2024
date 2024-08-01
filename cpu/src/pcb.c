#include "pcb.h"

t_pcb pcb;
bool proceso_ejecutando = false;

void recibir_pcb(){
    recibir_operacion(sockets_cpu.socket_kernel_dispatch);
    t_list *lista_pcb = recibir_paquete(sockets_cpu.socket_kernel_dispatch);
    t_pcb *pcb_ptr = desempaquetar_pcb(lista_pcb);
    
    pcb.pid = pcb_ptr->pid;
    pcb.pc = pcb_ptr->pc;
    pcb.quantum = pcb_ptr->quantum;
    pcb.registros = pcb_ptr->registros;

    regcpu = reg_gen_to_reg_cpu(pcb.registros);
    regcpu.PC = pcb.pc;

    proceso_ejecutando = true;
    list_destroy(lista_pcb);
}

void actualizar_pcb() {
    pcb.pc = regcpu.PC;
    pcb.registros = reg_cpu_to_reg_gen(regcpu);
}