#include "pcb.h"

t_paquete *empaquetar_pcb(t_pcb *pcb) {
    t_paquete *paquete = crear_paquete();
    agregar_a_paquete(paquete, &(pcb->pid), sizeof(pcb->pid));
    agregar_a_paquete(paquete, &(pcb->pc), sizeof(pcb->pc));
    agregar_a_paquete(paquete, &(pcb->quantum), sizeof(pcb->quantum));
    agregar_a_paquete(paquete, &(pcb->registros.AX), sizeof(pcb->registros.AX));
    agregar_a_paquete(paquete, &(pcb->registros.BX), sizeof(pcb->registros.BX));
    agregar_a_paquete(paquete, &(pcb->registros.CX), sizeof(pcb->registros.CX));
    agregar_a_paquete(paquete, &(pcb->registros.DX), sizeof(pcb->registros.DX));
    agregar_a_paquete(paquete, &(pcb->registros.EAX), sizeof(pcb->registros.EAX));
    agregar_a_paquete(paquete, &(pcb->registros.EBX), sizeof(pcb->registros.EBX));
    agregar_a_paquete(paquete, &(pcb->registros.ECX), sizeof(pcb->registros.ECX));
    agregar_a_paquete(paquete, &(pcb->registros.EDX), sizeof(pcb->registros.EDX));

    return paquete;
}

t_pcb *desempaquetar_pcb(t_list *lista) {
    t_pcb *pcb = malloc(sizeof(t_pcb));
    pcb->pid = *((int*) list_get(lista, 0));
    pcb->pc = *((int*) list_get(lista, 1));
    pcb->quantum = *((int*) list_get(lista, 2));
    pcb->registros.AX = *((int*) list_get(lista, 3));
    pcb->registros.BX = *((int*) list_get(lista, 4));
    pcb->registros.CX = *((int*) list_get(lista, 5));
    pcb->registros.DX = *((int*) list_get(lista, 6));
    pcb->registros.EAX = *((int*) list_get(lista, 7));
    pcb->registros.EBX = *((int*) list_get(lista, 8));
    pcb->registros.ECX = *((int*) list_get(lista, 9));
    pcb->registros.EDX = *((int*) list_get(lista, 10));

    return pcb;
}