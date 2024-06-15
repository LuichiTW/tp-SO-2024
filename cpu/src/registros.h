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

typedef enum{
    PROXIMAINSTRUCCION, //la memoria me devuelve un char con la proxima instruccion a ejecutar
    RESIZE, //reducir o aumentarel tamaño del proceso
    MOV_IN, //la memoria me devuelve un char con lo que habia en la direccion logica
    MOV_OUT, //le envio a memoria un char y lo escribe en la direccion fisica enviada
} op_codigo_conexion_memoria;

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

int tamanioRegistro(lista_registros_CPU);
void* obtenerRegistro(lista_registros_CPU);

#endif // !REGISTROSCPU