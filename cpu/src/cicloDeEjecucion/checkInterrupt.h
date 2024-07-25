#ifndef INTERRUPT
#define INTERRUPT

#include <stdlib.h>
#include <stdio.h>
#include <utils/conexiones/conexiones_servidor.h>
#include <utils/conexiones/conexiones_cliente.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/error.h>
#include <readline/readline.h>
#include <commons/string.h>
#include <semaphore.h>
#include "../pcb.h"

extern t_log *loggerPrincipal;
extern sem_t s_interrupcion;


typedef struct
{
	t_pcb *pcb;
	int socket_cliente_kernel_interrupt;
} t_parametroCheckInterrupt;


//extern t_pcb *pcb;

void funCheckInterrupt(t_pcb *pcb, int socket_cliente_kernel_interrupt);
void checkInterrupt(t_parametroCheckInterrupt *parametros);

#endif // !INTERRUPT