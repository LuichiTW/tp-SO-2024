#include "planificador.h"

t_queue *cola_new;
t_queue *cola_ready;
t_queue *cola_ready_aux;
t_queue *cola_blocked;
t_queue *cola_exit;
t_pcb *exec;

t_list *lista_procesos;

sem_t sem_planificacion_general;
sem_t sem_planificacion;


int ultimo_pid = 0;

void inicializar_colas() {
    cola_new = queue_create();
    cola_ready = queue_create();
    cola_ready_aux = queue_create();
    cola_blocked = queue_create();
    cola_exit = queue_create();

    lista_procesos = list_create();

    sem_init(&sem_planificacion_general, false, 1);
    sem_init(&sem_planificacion, false, 1);
}

void planificar() {
    pthread_t t;
    pthread_create(&t, NULL, (void*)realizar_planificacion, NULL);
    pthread_detach(t);
}

void realizar_planificacion() {
    sem_wait(&sem_planificacion_general);
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
    sem_post(&sem_planificacion_general);
}

void agregar_a_ready(t_pcb *pcb) {
    sem_wait(&sem_planificacion);

    t_estado_proceso *est = buscar_estado_proceso(pcb->pid);
    if (est->estado == ESTADO_NEW) {
        multiprogramacion_actual++;
    }
    est->estado = ESTADO_READY;
    queue_push(cola_ready, pcb);

    sem_post(&sem_planificacion);
}
void agregar_a_exec(t_pcb *pcb) {
    sem_wait(&sem_planificacion);
    
    t_estado_proceso *est = buscar_estado_proceso(pcb->pid);
    est->estado = ESTADO_EXEC;
    exec = pcb;

    sem_post(&sem_planificacion);
}
void agregar_a_exit(t_pcb *pcb) {
    sem_wait(&sem_planificacion);

    t_estado_proceso *est = buscar_estado_proceso(pcb->pid);
    multiprogramacion_actual--;
    est->estado = ESTADO_EXIT;
    queue_push(cola_exit, pcb);

    sem_post(&sem_planificacion);
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

    // Lo agrega a la lista general de procesos
    t_estado_proceso *est = malloc(sizeof(*est));
    est->pid = pcb->pid;
    est->estado = ESTADO_NEW;
    list_add(lista_procesos, est);

    return pcb;
}

void ejecutar_proceso(t_pcb *pcb) {
    exec = pcb;

    t_paquete *paquete = empaquetar_pcb(pcb);
    enviar_peticion(paquete, sockets.cpu_dispatch, CPU_EXEC_PROC);

    eliminar_paquete(paquete);
}


void finalizar_proceso(t_pcb *pcb) {
    t_paquete *paquete = crear_paquete();
    agregar_a_paquete(paquete, &(pcb->pid), sizeof(pcb->pid));
    
    enviar_peticion(paquete, sockets.memoria, MEM_FINALIZAR_PROCESO);
    eliminar_paquete(paquete);

    recibir_operacion(sockets.memoria);
    recibir_entero(sockets.memoria);

    sem_wait(&sem_planificacion);
    if (pcb == exec) {
        exec = NULL;
    }
    else {
        t_estado_proceso *est = buscar_estado_proceso(pcb->pid);
        t_queue *cola_actual = obtener_cola_por_estado(est->estado);
        if (cola_actual != NULL) list_remove_element(cola_actual->elements, pcb);
    }
    sem_post(&sem_planificacion);
    agregar_a_exit(pcb);
}


t_estado_proceso *buscar_estado_proceso(int pid) {
    bool es_el_buscado(t_estado_proceso *est) {
        return est->pid == pid;
    }
    return list_find(lista_procesos, (void *) es_el_buscado);
}
t_pcb *buscar_pcb_por_pid(int pid) {
    bool es_el_buscado(t_pcb *pcb) {
        return pcb->pid == pid;
    }

    t_estado_proceso *est = buscar_estado_proceso(pid);
    t_queue *cola = obtener_cola_por_estado(est->estado);

    return list_find(cola->elements, (void *) es_el_buscado);
}
t_queue *obtener_cola_por_estado(enum estado estado) {
    switch (estado) {
    case ESTADO_NEW:
        return cola_new;
        break;
    case ESTADO_READY:
        return cola_ready;
        break;
    case ESTADO_READY_AUX:
        return cola_ready_aux;
        break;
    case ESTADO_BLOCKED:
        return cola_blocked;
        break;
    case ESTADO_EXIT:
        return cola_exit;
        break;
    default:
        return NULL;
        break;
    }
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