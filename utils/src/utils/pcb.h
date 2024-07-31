#ifndef PCB_H
#define PCB_H

#include "conexiones/conexiones_cliente.h"
#include "conexiones/conexiones_servidor.h"
#include "registros.h"

typedef struct
{
    int pid;     // Identificador del proceso
    int pc;      // Contador de Programa
    int quantum; // Unidad de tiempo utilizada para el algoritmo VRR
    t_registros_uso_general registros; // Estado de los registros de uso general
} t_pcb;

t_paquete *empaquetar_pcb(t_pcb *pcb);
t_pcb *desempaquetar_pcb(t_list *lista);
#endif // PCB_H