#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H

#include <stdlib.h>
#include <commons/collections/queue.h>
#include <utils/pcb.h>
#include "conexion.h"
#include "config.h"


extern t_queue *cola_new;
extern t_queue *cola_ready;
extern t_queue *cola_ready_aux; // Para VRR
extern t_queue *cola_blocked;
extern t_queue *cola_exit;
extern t_pcb *exec;

void inicializar_colas();
void planificar();
t_pcb *crear_proceso();
void agregar_a_ready(t_pcb *pcb);
void ejecutar_proceso(t_pcb *pcb);

void log_cola_ready();

#endif // PLANIFICADOR_H