#ifndef REGISTROS_H
#define REGISTROS_H

#include <stdint.h>

typedef struct {
    uint8_t AX, BX, CX, DX;
    uint32_t EAX, EBX, ECX, EDX, SI, DI, PC;
} registros_cpu;

typedef struct {
    uint8_t AX, BX, CX, DX;
    uint32_t EAX, EBX, ECX, EDX;
} t_registros_uso_general;

//CANTIDAD DE REGISTROS Q TIENE EL CPU
typedef enum lista_registros_CPU{
    AX, BX, CX, DX,
    EAX, EBX, ECX, EDX,
    PC, SI, DI,
} lista_registros_CPU;

int tamanioRegistro(lista_registros_CPU registro);
registros_cpu reg_gen_to_reg_cpu(t_registros_uso_general reg_gen);
t_registros_uso_general reg_cpu_to_reg_gen(registros_cpu reg_cpu);

#endif // REGISTROS_H