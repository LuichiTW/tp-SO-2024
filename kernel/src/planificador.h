#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H

#include <stdlib.h>
#include <commons/collections/queue.h>
#include <utils/pcb.h>


extern t_queue *cola_new;
extern t_queue *cola_ready;
extern t_queue *cola_ready_aux; // Para VRR
extern t_queue *cola_blocked;
extern t_queue *cola_exit;

void inicializar_colas();
t_pcb *crear_proceso();

#endif // PLANIFICADOR_H