#include "funcionesexecute.h"
/* 
IO_GEN_SLEEP (Interfaz, Unidades de trabajo): Esta instrucción solicita al Kernel que se envíe a una interfaz de I/O a que realice un sleep por una cantidad de unidades de trabajo.
*/
void fSUM(enum lista_registros_CPU regLetraA, enum lista_registros_CPU regLetraB){
    void * regA = obtenerRegistro(regLetraA);
    int tamanioA = tamanioRegistro(regLetraA);
    void * regB = obtenerRegistro(regLetraB);
    int tamanioB = tamanioRegistro(regLetraB);
    
    if (regA==NULL || regB==NULL){
        //logger error
        // log_error(loggerPrincipal, "Uno de los registros del execute SUM no son validos");
        //enviar error al kernel
        //suspender proceso
        return;
    }
    //segun el tamanio del primer registro
    switch (tamanioA){
    case 1:
        //segun el tamanio del segundo registro
        switch (tamanioB){
        case 1: *(uint8_t*)regA+=*(uint8_t*)regB; break;
        case 4: *(uint8_t*)regA+=*(uint32_t*)regB; break;
        break;
        } break;
    case 4:
        //segun el tamanio del segundo registro
        switch (tamanioB){
        case 1: *(uint32_t*)regA+=*(uint8_t*)regB; break;
        case 4: *(uint32_t*)regA+=*(uint32_t*)regB; break;
        break;
        } break;
    }
}
void fSUB(enum lista_registros_CPU regLetraA, enum lista_registros_CPU regLetraB){
    void * regA = obtenerRegistro(regLetraA);
    int tamanioA = tamanioRegistro(regLetraA);
    void * regB = obtenerRegistro(regLetraB);
    int tamanioB = tamanioRegistro(regLetraB);
    
    if (regA==NULL || regB==NULL){
        //logger error
        // log_error(loggerPrincipal, "Uno de los registros del execute SUB no son validos");
        //enviar error al kernel
        //suspender proceso
        return;
    }
    //segun el tamanio del primer registro
    switch (tamanioA){
    case 1:
        //segun el tamanio del segundo registro
        switch (tamanioB){
        case 1: *(uint8_t*)regA-=*(uint8_t*)regB; break;
        case 4: *(uint8_t*)regA-=*(uint32_t*)regB; break;
        break;
        } break;
    case 4:
        //segun el tamanio del segundo registro
        switch (tamanioB){
        case 1: *(uint32_t*)regA-=*(uint8_t*)regB; break;
        case 4: *(uint32_t*)regA-=*(uint32_t*)regB; break;
        break;
        } break;
    }
}
void fSET(enum lista_registros_CPU regLetraA, int valor){
    void* regA = obtenerRegistro(regLetraA);
    int tamanioA = tamanioRegistro(regLetraA);
    
    if (regA==NULL){
        //logger error
        // log_error(loggerPrincipal, "El registro del execute SET no es valido");
        //enviar error al kernel
        //suspender proceso
        return;
    }
    //segun el tamanio del primer registro
    switch (tamanioA){
    case 1:
        //segun el tamanio del segundo registro
        *(uint8_t*)regA=valor; break;
    case 4:
        //segun el tamanio del segundo registro
        *(uint32_t*)regA=valor; break;
    }
}
void fJNZ(enum lista_registros_CPU registroLetra, uint32_t instruccion){
    void * reg = obtenerRegistro(registroLetra);
    if (*(int*)reg != 0) {
        regcpu.PC = instruccion;
        return;
    } else {
        log_error(loggerPrincipal,"Error al hacer salto de instruccion, la variable es igual a 0");
        return;
    }
    
}
void fMOV_IN(enum lista_registros_CPU Datos, enum lista_registros_CPU Dirrecion){ //!OBLIGATORIO

}
void fMOV_OUT(enum lista_registros_CPU Dirrecion, enum lista_registros_CPU Datos){//!OBLIGATORIO

}
void fRESIZE(int tamanho){//!OBLIGATORIO

}
void fCOPY_STRING(int ramanho){//!OBLIGATORIO

}
void fWAIT(char recurso[]){

}
void fSIGNAL(char recurso[]){

}
void fIO_GEN_SLEEP(char interface[], int unidadesDeTrabajo){//!OBLIGATORIO

}
void fIO_STDIN_READ(char interface[], enum lista_registros_CPU Dirrecion, enum lista_registros_CPU Tamanho){//!OBLIGATORIO

}
void fIO_STDOUT_WRITE(char interface[], enum lista_registros_CPU Dirrecion, enum lista_registros_CPU Tamanho){//!OBLIGATORIO

}
void fIO_FS_CREATE(char interface[], char NombreArchivo[]){

}
void fIO_FS_DELETE(char interface[], char NombreArchivo[]){

}
void fIO_FS_TRUNCATE(char interface[], char NombreArchivo[], enum lista_registros_CPU Tamanho){

}
void fIO_FS_WRITE(char interface[], char NombreArchivo[], enum lista_registros_CPU Direccion, enum lista_registros_CPU Tamanho, enum lista_registros_CPU PunteroArchivo){

}
void fIO_FS_READ(char interface[], char NombreArchivo[], enum lista_registros_CPU Direccion, enum lista_registros_CPU Tamanho, enum lista_registros_CPU PunteroArchivo){

}
void fEXIT(){

}

