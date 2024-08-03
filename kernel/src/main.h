#ifndef MAIN_KERNEL

#define MAIN_KERNEL


// Algunas se podrían sacar en el futuro
#include <stdlib.h>
#include <stdio.h>
#include <utils/conexiones/conexiones_servidor.h>
#include <utils/conexiones/conexiones_cliente.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/error.h>
#include <readline/readline.h>
#include <string.h>
#include <commons/temporal.h>
#include <commons/temporal.h>

#include <semaphore.h>
#include <pthread.h>
#include "config.h"
#include "conexion.h"
#include "planificador.h"
#include "consola.h"
#include "escucha_dispatch.h"
#include "io.h"

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



//extern int i = 1;



void atender_instruccion_valida(char*leido, t_log*logger, int conexion_cpu_dispatch,int conexion_cpu_interrupt,int conexion_memoria,Cola*colaNEW,Cola*colaREADY,Cola*colaFIFO,Cola*colaRR,Cola*colaVRR,Cola*colaEXIT);

void iniciar_consola_interactiva(t_log*logger,int conexion_cpu_dispatch,int conexion_cpu_interrupt,int conexion_memoria,Cola*colaNEW,Cola*colaREADY,Cola*colaFIFO,Cola*colaRR,Cola*colaVRR,Cola*colaEXIT);

bool validacion_de_instruccion_de_consola(char* leido, t_log* logger);

void iniciar_proceso(char*path,int PID,int conexion_cpu,int conexion_memoria,Cola*colaNEW);

void enviar_pcb(pcb proceso,int conexion_cpu);

void encolarColaNEW(pcb,Cola*colaNEW);

void encolarColaREADY(Cola*colaNEW,Cola*colaREADY,Cola*colaFIFO,Cola*colaRR,Cola*colaVRR);

void encolarColaFIFO(Cola*colaREADY,Cola*colaFIFO);

void encolarColaRR(Cola*colaREADY,Cola*colaRR);

void encolarColaVRR(Cola*colaREADY,Cola*colaVRR);

void encolarColaVRRAux(pcb ProcesoNuevo,Cola*colaVRRAux);

void encolarAlFinal(Cola*cola);

void listarProcesos(Cola*cola,char*estado,t_log*logger);



#endif // MAIN_KERNEL