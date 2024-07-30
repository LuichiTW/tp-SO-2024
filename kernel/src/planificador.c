#include "planificador.h"

t_queue *cola_new;
t_queue *cola_ready;
t_queue *cola_ready_aux;
t_queue *cola_blocked;
t_queue *cola_exit;

void inicializar_colas() {
    cola_new = queue_create();
    cola_ready = queue_create();
    cola_ready_aux = queue_create();
    cola_blocked = queue_create();
    cola_exit = queue_create();
}