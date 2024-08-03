#include "funcionesexecute.h"
/* 
IO_GEN_SLEEP (Interfaz, Unidades de trabajo): Esta instrucción solicita al Kernel que se envíe a una interfaz de I/O a que realice un sleep por una cantidad de unidades de trabajo.
*/

// TODO enviar a kernel out of memory (RESIZE)
// TODO testear bien COPY_STRING (usando IO)

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
    }
}
void fMOV_IN(enum lista_registros_CPU Datos, enum lista_registros_CPU Direccion){
    
    char *registro = (char*) obtenerRegistro(Datos);
    int dir_logica = *((int *) obtenerRegistro(Direccion));
    int tam_registro = tamanioRegistro(Datos);
    t_lista_dir_fisicas lista_dir = obtener_direcciones_fisicas(dir_logica, tam_registro);

    for (int i = 0; i < lista_dir.cant_paginas; i++) {
        t_paquete *paquete = crear_paquete();
        int dir_fisica = lista_dir.dir_fisicas[i];
        int tam_dato = lista_dir.bytes_por_pag[i];

        agregar_a_paquete(paquete, &dir_fisica, sizeof(dir_fisica));
        agregar_a_paquete(paquete, &tam_dato, sizeof(tam_dato));

        enviar_peticion(paquete, sockets_cpu.socket_memoria, MEM_LEER_MEMORIA);
        eliminar_paquete(paquete);

        recibir_operacion(sockets_cpu.socket_memoria);
        char *valor = recibir_msg(sockets_cpu.socket_memoria);

        memcpy(registro, valor, tam_dato);
        registro += tam_dato;

        free(valor);
    }
}
void fMOV_OUT(enum lista_registros_CPU Direccion, enum lista_registros_CPU Datos){

    char *registro = (char*) obtenerRegistro(Datos);
    int dir_logica = *((int *) obtenerRegistro(Direccion));
    int tam_registro = tamanioRegistro(Datos);
    t_lista_dir_fisicas lista_dir = obtener_direcciones_fisicas(dir_logica, tam_registro);

    for (int i = 0; i < lista_dir.cant_paginas; i++) {
        t_paquete *paquete = crear_paquete();
        int dir_fisica = lista_dir.dir_fisicas[i];
        int tam_dato = lista_dir.bytes_por_pag[i];

        agregar_a_paquete(paquete, &dir_fisica, sizeof(dir_fisica));
        agregar_a_paquete(paquete, &tam_dato, sizeof(tam_dato));
        agregar_a_paquete(paquete, registro, tam_dato);

        enviar_peticion(paquete, sockets_cpu.socket_memoria, MEM_ESCRIBIR_MEMORIA);
        eliminar_paquete(paquete);
        
        recibir_operacion(sockets_cpu.socket_memoria);
        recibir_entero(sockets_cpu.socket_memoria);
        registro += tam_dato;
    }
}
void fRESIZE(int tamanho){

    t_paquete *paquete = crear_paquete();
    agregar_a_paquete(paquete, &(pcb.pid), sizeof(int));
    agregar_a_paquete(paquete, &tamanho, sizeof(int));
    enviar_peticion(paquete, sockets_cpu.socket_memoria, MEM_RESIZE_PROCESO);
    eliminar_paquete(paquete);

    recibir_operacion(sockets_cpu.socket_memoria);
    int rta = recibir_entero(sockets_cpu.socket_memoria);
    if (rta == 1) { // Out of memory sería
        // TODO enviar a kernel o lo que sea
    }
}
void fCOPY_STRING(int tamanho){
    int di_dir_logica = *((int*) obtenerRegistro(DI));
    int si_dir_logica = *((int*) obtenerRegistro(SI));

    char *str = string_new(); // String que copio de DI a SI

    // Obtener string
    t_lista_dir_fisicas lista_dir_di = obtener_direcciones_fisicas(di_dir_logica, tamanho);
    
    for (int i = 0; i < lista_dir_di.cant_paginas; i++) {
        int dir_fisica = lista_dir_di.dir_fisicas[i];
        int tam_dato = lista_dir_di.bytes_por_pag[i];

        t_paquete *paquete = crear_paquete();
        agregar_a_paquete(paquete, &dir_fisica, sizeof(dir_fisica));
        agregar_a_paquete(paquete, &tam_dato, sizeof(tam_dato));

        enviar_peticion(paquete, sockets_cpu.socket_memoria, MEM_LEER_MEMORIA);
        eliminar_paquete(paquete);

        recibir_operacion(sockets_cpu.socket_memoria);
        char *str_rta = recibir_msg(sockets_cpu.socket_memoria);
        string_append(&str, str_rta);

        free(str_rta);
    }

    // Escribir string
    char *strptr = str; // Puntero que va a ir avanzando por el string para enviarlo
    t_lista_dir_fisicas lista_dir_si = obtener_direcciones_fisicas(si_dir_logica, tamanho);

    for (int i = 0; i < lista_dir_si.cant_paginas; i++) {
        int dir_fisica = lista_dir_si.dir_fisicas[i];
        int tam_dato = lista_dir_si.bytes_por_pag[i];

        t_paquete *paquete = crear_paquete();
        agregar_a_paquete(paquete, &dir_fisica, sizeof(dir_fisica));
        agregar_a_paquete(paquete, &tam_dato, sizeof(tam_dato));
        agregar_a_paquete(paquete, strptr, tam_dato);
        
        enviar_peticion(paquete, sockets_cpu.socket_memoria, MEM_ESCRIBIR_MEMORIA);
        eliminar_paquete(paquete);

        recibir_operacion(sockets_cpu.socket_memoria);
        recibir_msg(sockets_cpu.socket_memoria);
        
        strptr += tam_dato;
    }
    
    free(str);
}
void fWAIT(char recurso[]){

}
void fSIGNAL(char recurso[]){

}
void fIO_GEN_SLEEP(char interface[], int unidadesDeTrabajo){
    char *motivo_desalojo = string_from_format("IO_GEN_SLEEP %s %i", interface, unidadesDeTrabajo);
    devolver_contexto_ejecucion(motivo_desalojo);
    free(motivo_desalojo);
}
void fIO_STDIN_READ(char interface[], enum lista_registros_CPU Direccion, enum lista_registros_CPU Tamanio){//!OBLIGATORIO
    int dir_logica = *((int *) obtenerRegistro(Direccion));
    int registro_tamanios = *((int *) obtenerRegistro(Tamanio));
    t_lista_dir_fisicas lista_dir = obtener_direcciones_fisicas(dir_logica, registro_tamanios);
    char *array_direcciones = array_a_string(lista_dir.dir_fisicas,lista_dir.cant_paginas);
    char *array_tamanios = array_a_string(lista_dir.bytes_por_pag,lista_dir.cant_paginas);

    char *motivo_desalojo = string_from_format("IO_STDIN_READ %s %s %d", interface, array_direcciones, array_tamanios);
    devolver_contexto_ejecucion(motivo_desalojo);
    free(motivo_desalojo);
}
void fIO_STDOUT_WRITE(char interface[], enum lista_registros_CPU Direccion, enum lista_registros_CPU Tamanio){//!OBLIGATORIO
    int dir_logica = *((int *) obtenerRegistro(Direccion));
    int registro_tamanios = *((int *) obtenerRegistro(Tamanio));
    t_lista_dir_fisicas lista_dir = obtener_direcciones_fisicas(dir_logica, registro_tamanios);
    char *array_direcciones = array_a_string(lista_dir.dir_fisicas,lista_dir.cant_paginas);
    char *array_tamanios = array_a_string(lista_dir.bytes_por_pag,lista_dir.cant_paginas);

    char *motivo_desalojo = string_from_format("IO_STDIN_WRITE %s %s %d", interface, array_direcciones, array_);
    devolver_contexto_ejecucion(motivo_desalojo);
    free(motivo_desalojo);
}
void fIO_FS_CREATE(char interface[], char NombreArchivo[]){
    char *motivo_desalojo = string_from_format("IO_FS_CREATE %s %s", interface, NombreArchivo);
    devolver_contexto_ejecucion(motivo_desalojo);
    free(motivo_desalojo);
}
void fIO_FS_DELETE(char interface[], char NombreArchivo[]){
    char *motivo_desalojo = string_from_format("IO_FS_DELETE %s %s", interface, NombreArchivo);
    devolver_contexto_ejecucion(motivo_desalojo);
    free(motivo_desalojo);
}
void fIO_FS_TRUNCATE(char interface[], char NombreArchivo[], enum lista_registros_CPU Tamanio){
    int tam_registro = *((int *) obtenerRegistro(Tamanio));

    char *motivo_desalojo = string_from_format("IO_FS_TRUNCATE %s %s %s", interface, NombreArchivo,tam_registro);
    devolver_contexto_ejecucion(motivo_desalojo);
    free(motivo_desalojo);
}
void fIO_FS_WRITE(char interface[], char NombreArchivo[], enum lista_registros_CPU Direccion, enum lista_registros_CPU Tamanio, enum lista_registros_CPU PunteroArchivo){
    int dir_logica = *((int *) obtenerRegistro(Direccion));
    int registro_tamanios = *((int *) obtenerRegistro(Tamanio));
    int puntero = *((int *) obtenerRegistro(PunteroArchivo));
    t_lista_dir_fisicas lista_dir = obtener_direcciones_fisicas(dir_logica, registro_tamanios);
    char *array_direcciones = array_a_string(lista_dir.dir_fisicas,lista_dir.cant_paginas);
    char *array_tamanios = array_a_string(lista_dir.bytes_por_pag,lista_dir.cant_paginas);

    char *motivo_desalojo = string_from_format("IO_FS_WRITE %s %s %s %s", interface, NombreArchivo,array_direcciones,array_tamanios,puntero);
    devolver_contexto_ejecucion(motivo_desalojo);
    free(motivo_desalojo);
}
void fIO_FS_READ(char interface[], char NombreArchivo[], enum lista_registros_CPU Direccion, enum lista_registros_CPU Tamanio, enum lista_registros_CPU PunteroArchivo){
    int dir_logica = *((int *) obtenerRegistro(Direccion));
    int registro_tamanios = *((int *) obtenerRegistro(Tamanio));
    int puntero = *((int *) obtenerRegistro(PunteroArchivo));
    t_lista_dir_fisicas lista_dir = obtener_direcciones_fisicas(dir_logica, registro_tamanios);
    char *array_direcciones = array_a_string(lista_dir.dir_fisicas,lista_dir.cant_paginas);
    char *array_tamanios = array_a_string(lista_dir.bytes_por_pag,lista_dir.cant_paginas);

    char *motivo_desalojo = string_from_format("IO_FS_READ %s %s %s %s", interface, NombreArchivo,array_direcciones,array_tamanios,puntero);
    devolver_contexto_ejecucion(motivo_desalojo);
    free(motivo_desalojo);
}
void fEXIT(){
    devolver_contexto_ejecucion("EXIT");
}

char *array_a_string(int *direcciones,int tamanio){
    char *array;
    for(int i=0;i<tamanio;i++){
        if ((i > 0)) {
            string_append(array, ",");
        }
        char *numero = string_itoa(direcciones[i]);
        string_append(array,numero);
        free(numero);
    }
    return array;
}

