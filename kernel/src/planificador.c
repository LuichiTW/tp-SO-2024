#include "planificador.h"

t_queue *cola_new;
t_queue *cola_ready;
t_queue *cola_ready_aux;
t_queue *cola_blocked;
t_queue *cola_exit;
t_pcb *exec;

// ! Revisar planificacion_activada por espera activa
// ? Para eso podría usar semáforos

int ultimo_pid = 0;

void inicializar_colas() {
    cola_new = queue_create();
    cola_ready = queue_create();
    cola_ready_aux = queue_create();
    cola_blocked = queue_create();
    cola_exit = queue_create();
}

void planificar() {
    // NEW -> READY
    while (multiprogramacion_actual < config_kernel.grado_multiprogramacion && queue_size(cola_new) > 0) {
        t_pcb *pcb_ready = (t_pcb*) queue_pop(cola_new);
        agregar_a_ready(pcb_ready);
    }

    if (exec == NULL && !queue_is_empty(cola_ready)) {
        // TODO chequear con cola ready_aux para VRR
        t_pcb *pcb_exec = (t_pcb*) queue_pop(cola_ready);
        ejecutar_proceso(pcb_exec);
    }
}

t_pcb *crear_proceso() {
    t_pcb *pcb = malloc(sizeof(pcb));

    ultimo_pid++;
    pcb->pid = ultimo_pid;
    pcb->pc = 0;
    pcb->quantum = 0;
    pcb->registros.AX = 0;
    pcb->registros.BX = 0;
    pcb->registros.CX = 0;
    pcb->registros.DX = 0;
    pcb->registros.EAX = 0;
    pcb->registros.EBX = 0;
    pcb->registros.ECX = 0;
    pcb->registros.EDX = 0;

    return pcb;
}

void agregar_a_ready(t_pcb *pcb) {
    queue_push(cola_ready, pcb);
    multiprogramacion_actual++;
}

void ejecutar_proceso(t_pcb *pcb) {
    exec = pcb;

    t_paquete *paquete = empaquetar_pcb(pcb);
    enviar_peticion(paquete, sockets.cpu_dispatch, CPU_EXEC_PROC);
}