#include "funcionesexecute.h"
/* 
IO_GEN_SLEEP (Interfaz, Unidades de trabajo): Esta instrucción solicita al Kernel que se envíe a una interfaz de I/O a que realice un sleep por una cantidad de unidades de trabajo.
*/
void fSUM(enum lista_registros_CPU regLetraA, enum lista_registros_CPU regLetraB){
    void * regA = obtenerRegistro(regLetraA);
    int tamanioA = tamanioRegistro(regLetraA);
    void * regB = obtenerRegistro(regLetraB);
    int tamanioB = tamanioRegistro(regLetraB);
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
void fMOV_IN(enum lista_registros_CPU Datos, enum lista_registros_CPU Direccion){ //!OBLIGATORIO
    t_paquete *paquete = crear_paquete();
    int dir_fisica;
    int tam_registro;
    char *registro;

    dir_fisica = obtener_direccion_fisica(separar_dir_logica(Direccion));
    tam_registro = tamanioRegistro(Datos);

    agregar_a_paquete(paquete, &dir_fisica, sizeof(dir_fisica));
    agregar_a_paquete(paquete, &tam_registro, sizeof(tam_registro));

    enviar_peticion(paquete, sockets_cpu.socket_memoria, MEM_LEER_MEMORIA);
    eliminar_paquete(paquete);

    char *valor = recibir_msg(sockets_cpu.socket_memoria);
    registro = (char*) obtenerRegistro(Datos);

    strncpy(registro, valor, tam_registro);
    free(valor);
}
void fMOV_OUT(enum lista_registros_CPU Direccion, enum lista_registros_CPU Datos){//!OBLIGATORIO
    t_paquete *paquete = crear_paquete();
    int dir_fisica;
    int tam_registro;
    char *registro;

    dir_fisica = obtener_direccion_fisica(separar_dir_logica(Direccion));
    tam_registro = tamanioRegistro(Datos);
    registro = (char*) obtenerRegistro(Datos);

    agregar_a_paquete(paquete, &dir_fisica, sizeof(dir_fisica));
    agregar_a_paquete(paquete, &tam_registro, sizeof(tam_registro));
    agregar_a_paquete(paquete, &registro, tam_registro);

    enviar_peticion(paquete, sockets_cpu.socket_memoria, MEM_ESCRIBIR_MEMORIA);
}
void fRESIZE(int tamanho){//!OBLIGATORIO
    t_paquete *paquete = crear_paquete();

    agregar_a_paquete(paquete, &(pcb->PID), sizeof(int));
    agregar_a_paquete(paquete, &tamanho, sizeof(int));

    enviar_peticion(paquete, sockets_cpu.socket_memoria, MEM_RESIZE_PROCESO);
    eliminar_paquete(paquete);
    int rta = recibir_entero(sockets_cpu.socket_memoria);
    if (rta == 1) { // Out of memory sería
        // TODO enviar a kernel o lo que sea
    }
}
void fCOPY_STRING(int tamanho){//!OBLIGATORIO

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

