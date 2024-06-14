#ifndef REGISTROSCPU
#define REGISTROSCPU

// #include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include <utils/conexiones/conexiones_servidor.h>
#include <utils/conexiones/conexiones_cliente.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/error.h>
#include <readline/readline.h>


//REGISTROS QUE TIENE EL CPU
typedef struct {
    uint8_t AX, BX, CX, DX;
    uint32_t EAX, EBX, ECX, EDX, SI, DI, PC;
} registros_cpu;


extern registros_cpu regcpu;
//CANTIDAD DE REGISTROS Q TIENE EL CPU
typedef enum lista_registros_CPU{
    AX, BX, CX, DX,
    EAX, EBX, ECX, EDX,
    PC, SI, DI,
} lista_registros_CPU;

int tamanioRegistro(lista_registros_CPU);
void* obtenerRegistro(lista_registros_CPU);

#endif // !REGISTROSCPU