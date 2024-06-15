#ifndef MAIN_CPU
#define MAIN_CPU

#include <stdlib.h>
#include <stdio.h>
#include <utils/conexiones/conexiones_servidor.h>
#include <utils/conexiones/conexiones_cliente.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/error.h>
#include <readline/readline.h>
#include <pthread.h>
#include <semaphore.h>

#include "registros.h"
#include "cicloDeEjecucion/decode.h"
#include "cicloDeEjecucion/execute.h"
#include "listaInstrucciones.h"
#include "cicloDeEjecucion/fetch.h"
#include "cicloDeEjecucion/checkInterrupt.h"
#include "config.h"

typedef struct
    {
        int PID;     // Identificador del proceso
        int PC;      // Contador de Programa
        int Quantum; // Unidad de tiempo utilizada para el algoritmo VRR
        
    }t_pcb;

extern t_pcb *pcb;

t_pcb *recibir_pcb(int socket_cliente_kernel_dispatch, int socket_cliente_kernel_interrupt);

#endif // !MAIN_CPU