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

struct NodoColaPCBS*ListaFIFO=NULL;
int pid_pcb=0;

void atender_instruccion_valida(char* leido,t_log*logger,int);
void iniciar_consola_interactiva(t_log* logger,int);
bool validacion_de_instruccion_de_consola(char* leido, t_log* logger);
void iniciar_proceso(char*path,int PID,int conexion_cpu,int conexion_memoria);
void enviar_pcb(pcb proceso,int conexion_cpu);
//void encolarColaNEW(pcb);

#endif // !MAIN_KERNEL
