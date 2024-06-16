#ifndef MAIN_KERNEL
#define MAIN_KERNEL

#include <stdlib.h>
#include <stdio.h>
#include <utils/conexiones/conexiones_servidor.h>
#include <utils/conexiones/conexiones_cliente.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/error.h>
#include <readline/readline.h>
#include <pthread.h>
#include <string.h>

typedef struct
    {
        int PID;     // Identificador del proceso
        int PC;      // Contador de Programa
        int Quantum; // Unidad de tiempo utilizada para el algoritmo VRR
        //necesito el tipo de dato de los registros que vienen del cpu
    }pcb;
typedef struct 
    {
        pcb PCBS;
        struct NodoColaPCBS*sig;
    }NodoColaPCBS;

typedef struct 
{
    NodoColaPCBS*primero;
    NodoColaPCBS*ultimo;
}Cola;

void atender_instruccion_valida(char* leido,t_log*logger,int,int c,Cola*colaNEW);
void iniciar_consola_interactiva(t_log* logger,int,int a,Cola*colaNEW);
bool validacion_de_instruccion_de_consola(char* leido, t_log* logger);
void iniciar_proceso(char*path,int PID,int conexion_cpu,int conexion_memoria,Cola*colaNEW);
void enviar_pcb(pcb proceso,int conexion_cpu);
void encolarColaNEW(pcb,Cola*colaNEW);

#endif // !MAIN_KERNEL
