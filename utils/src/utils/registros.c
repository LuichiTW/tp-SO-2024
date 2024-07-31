#include "registros.h"

//dado el nombre de un registro de la lista, retorno 1 o 4 segun la cantidad de bytes q tiene el registro
int tamanioRegistro(lista_registros_CPU registro) {
    
    if (registro >= AX && registro <= DX) {
        return 1;
    } else if (registro >= EAX && registro <= DI) {
        return 4;
    } else {
        return -1;
    }
}


registros_cpu reg_gen_to_reg_cpu(t_registros_uso_general reg_gen) {
    registros_cpu reg_cpu;
    reg_cpu.AX = reg_gen.AX;
    reg_cpu.BX = reg_gen.BX;
    reg_cpu.CX = reg_gen.CX;
    reg_cpu.DX = reg_gen.DX;
    reg_cpu.EAX = reg_gen.EAX;
    reg_cpu.EBX = reg_gen.EBX;
    reg_cpu.ECX = reg_gen.ECX;
    reg_cpu.EDX = reg_gen.EDX;
    reg_cpu.PC = 0;
    reg_cpu.SI = 0;
    reg_cpu.DI = 0;
    return reg_cpu;
}

t_registros_uso_general reg_cpu_to_reg_gen(registros_cpu reg_cpu) {
    t_registros_uso_general reg_gen;
    reg_gen.AX = reg_cpu.AX;
    reg_gen.BX = reg_cpu.BX;
    reg_gen.CX = reg_cpu.CX;
    reg_gen.DX = reg_cpu.DX;
    reg_gen.EAX = reg_cpu.EAX;
    reg_gen.EBX = reg_cpu.EBX;
    reg_gen.ECX = reg_cpu.ECX;
    reg_gen.EDX = reg_cpu.EDX;
    return reg_gen;
}