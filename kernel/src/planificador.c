#include "planificador.h"

t_queue *cola_new;
t_queue *cola_ready;
t_queue *cola_ready_aux;
t_queue *cola_blocked;
t_queue *cola_exit;
t_pcb *exec;

// ! Revisar planificacion_activada por espera activa
// ? Para eso podría usar semáforos
// Afectar también a finalizar_proceso

int ultimo_pid = 0;

void inicializar_colas() {
    cola_new = queue_create();
    cola_ready = queue_create();
    cola_ready_aux = queue_create();
    cola_blocked = queue_create();
    cola_exit = queue_create();
}

void planificar() {
    t_log *logger = kernel_logger();

    // NEW -> READY
    bool ingreso_a_ready = false;
    while (multiprogramacion_actual < config_kernel.grado_multiprogramacion && queue_size(cola_new) > 0) {
        t_pcb *pcb_ready = (t_pcb*) queue_pop(cola_new);
        agregar_a_ready(pcb_ready);
        log_info(logger, "PID: %i - Estado Anterior: NEW - Estado Actual: READY", pcb_ready->pid);
        ingreso_a_ready = true;
    }
    if (ingreso_a_ready) {
        log_cola_ready();
    }

    // READY -> EXEC
    if (exec == NULL && !queue_is_empty(cola_ready)) {
        // TODO chequear con cola ready_aux para VRR (y cambiar el log)
        t_pcb *pcb_exec = (t_pcb*) queue_pop(cola_ready);
        ejecutar_proceso(pcb_exec);
        log_info(logger, "PID: %i - Estado Anterior: READY - Estado Actual: EXEC", pcb_exec->pid);
    }

    log_destroy(logger);
}

t_pcb *crear_proceso() {
    t_pcb *pcb = malloc(sizeof(*pcb));

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

    eliminar_paquete(paquete);
}


void finalizar_proceso(t_pcb *pcb, t_queue *cola_actual) {
    t_paquete *paquete = crear_paquete();
    agregar_a_paquete(paquete, &(pcb->pid), sizeof(pcb->pid));
    
    enviar_peticion(paquete, sockets.memoria, MEM_FINALIZAR_PROCESO);
    eliminar_paquete(paquete);

    recibir_operacion(sockets.memoria);
    recibir_entero(sockets.memoria);

    if (pcb == exec) {
        exec = NULL;
    }
    else {
        if (cola_actual != NULL) list_remove_element(cola_actual->elements, pcb);
    }
    queue_push(cola_exit, pcb);
}



void log_cola_ready() {
    t_log *logger = kernel_logger();
    int cant_proc_ready = queue_size(cola_ready);
    char *lista_pid = string_new();
    for (int i = 0; i < cant_proc_ready; i++) {
        if (i > 0) {
            string_append(&lista_pid, ",");
        }
        t_pcb *pcb = list_get(cola_ready->elements, i);
        char *pid_str = string_itoa(pcb->pid);
        string_append(&lista_pid, pid_str);

        free(pid_str);
    }
    log_info(logger, "Cola Ready: [%s]", lista_pid);
    
    free(lista_pid);
    log_destroy(logger);
}