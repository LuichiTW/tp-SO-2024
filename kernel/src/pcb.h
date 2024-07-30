#ifndef PCB_H
#define PCB_H

#include <stdint.h>

typedef struct {
    uint8_t AX, BX, CX, DX;
    uint32_t EAX, EBX, ECX, EDX;
} t_registros_uso_general;

typedef struct
{
    int pid;     // Identificador del proceso
    int pc;      // Contador de Programa
    int quantum; // Unidad de tiempo utilizada para el algoritmo VRR
    t_registros_uso_general registros; // Estado de los registros de uso general
} t_pcb;

#endif // PCB_H