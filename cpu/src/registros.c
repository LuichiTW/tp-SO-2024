#include "registros.h"

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