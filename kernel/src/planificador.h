#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H

#include "commons/collections/queue.h"
#include "pcb.h"


extern t_queue *cola_new;
extern t_queue *cola_ready;
extern t_queue *cola_ready_aux; // Para VRR
extern t_queue *cola_blocked;

void inicializar_colas();

#endif // PLANIFICADOR_H