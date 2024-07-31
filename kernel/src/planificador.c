#include "planificador.h"

t_queue *cola_new;
t_queue *cola_ready;
t_queue *cola_ready_aux;
t_queue *cola_blocked;
t_queue *cola_exit;

int ultimo_pid = 0;

void inicializar_colas() {
    cola_new = queue_create();
    cola_ready = queue_create();
    cola_ready_aux = queue_create();
    cola_blocked = queue_create();
    cola_exit = queue_create();
}

t_pcb *crear_proceso() {
    t_pcb *pcb = malloc(sizeof(pcb));

    ultimo_pid++;
    pcb->pid = ultimo_pid;
    pcb->pc = 0;
    pcb->quantum = 0;
    pcb->registros = {0};

    queue_push(cola_new, pcb);
    return pcb;
}