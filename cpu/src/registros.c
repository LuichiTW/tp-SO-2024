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


//dado el nombre de un registro de la lista, retorna el registro solicitado, si no existe retorna NULL
void* obtenerRegistro(lista_registros_CPU registro) {
    void* listaRegistros[11] = {
        &regcpu.AX, &regcpu.BX, &regcpu.CX, &regcpu.DX, &regcpu.EAX, &regcpu.EBX, &regcpu.ECX, &regcpu.EDX, &regcpu.PC, &regcpu.SI, &regcpu.DI
    };
    //comprobamos que el registro solicitado esta en la lista.
    if(registro >= 0 && registro < 11){
        return(listaRegistros[registro]);
    }
    else{
        return NULL;
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