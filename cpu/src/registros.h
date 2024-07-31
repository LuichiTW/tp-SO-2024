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
#include <utils/registros.h>

extern registros_cpu regcpu;

typedef enum{
    EXIT, //FIN DE UN PROCESO POR INSTRUCCION O POR ERROR
    WAIT, 
    SIGNAL,
    IOGENSLEEP,
    IOSTDINREAD,
    IOSTDOUTWRITE,
    IOFSCREATE,
    IOFSDELETE,
    IOFSTRUNCATE,
    IOFSWRITE,
    IOFSREAD,
} op_codigo_conexion_kernel;

void* obtenerRegistro(lista_registros_CPU);

#endif // !REGISTROSCPU