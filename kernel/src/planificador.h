#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H

#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <commons/collections/queue.h>
#include <utils/pcb.h>
#include "conexion.h"
#include "config.h"

enum estado {
    ESTADO_NEW,
    ESTADO_READY,
    ESTADO_READY_AUX,
    ESTADO_EXEC,
    ESTADO_BLOCKED,
    ESTADO_EXIT
};

typedef struct {
    int pid;
    enum estado estado;
} t_estado_proceso;

// Lista con los procesos junto a su estado.
extern t_list *lista_procesos;

extern t_queue *cola_new;
extern t_queue *cola_ready;
extern t_queue *cola_ready_aux; // Para VRR
extern t_queue *cola_blocked;
extern t_queue *cola_exit;
extern t_pcb *exec;

extern sem_t sem_planificacion_general;
extern sem_t sem_planificacion;

void inicializar_colas();
void planificar();
void realizar_planificacion();

void agregar_a_ready(t_pcb *pcb);
void agregar_a_exec(t_pcb *pcb);
void agregar_a_exit(t_pcb *pcb);

t_pcb *crear_proceso();
void ejecutar_proceso(t_pcb *pcb);
void finalizar_proceso(t_pcb *pcb);

t_estado_proceso *buscar_estado_proceso(int pid);
t_pcb *buscar_pcb_por_pid(int pid);
t_queue *obtener_cola_por_estado(enum estado estado);

void log_cola_ready();

#endif // PLANIFICADOR_H