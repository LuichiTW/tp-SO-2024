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

t_temporal *temporal_global;
int64_t tiempo_inicio_cuenta_q;

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

    if (config_kernel.algoritmo_planificacion != ALGO_FIFO) {
        temporal_global = temporal_create();
    }
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
        log_cola_ready(false);
    }

    // READY_AUX -> EXEC
    if (exec == NULL && !queue_is_empty(cola_ready_aux)) {
        t_pcb *pcb_exec = (t_pcb*) queue_pop(cola_ready_aux);
        ejecutar_proceso(pcb_exec);
        log_info(logger, "PID: %i - Estado Anterior: READY_AUX - Estado Actual: EXEC", pcb_exec->pid);
    }

    // READY -> EXEC
    if (exec == NULL && !queue_is_empty(cola_ready)) {
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
void agregar_a_ready_aux(t_pcb *pcb) {
    sem_wait(&sem_planificacion);

    t_estado_proceso *est = buscar_estado_proceso(pcb->pid);
    est->estado = ESTADO_READY_AUX;
    queue_push(cola_ready_aux, pcb);

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
    est->motivo_bloqueo = BLOQUEO_NINGUNO;
    list_add(lista_procesos, est);

    return pcb;
}

void ejecutar_proceso(t_pcb *pcb) {
    agregar_a_exec(pcb);

    t_paquete *paquete = empaquetar_pcb(pcb);
    enviar_peticion(paquete, sockets.cpu_dispatch, CPU_EXEC_PROC);

    eliminar_paquete(paquete);

    if (config_kernel.algoritmo_planificacion != ALGO_FIFO) {
        tiempo_inicio_cuenta_q = temporal_gettime(temporal_global);

        struct arg_contar_quantum *args = malloc(sizeof(*args));
        args->tiempo_inicio = tiempo_inicio_cuenta_q;
        args->quantum = config_kernel.quantum - pcb->quantum;

        pthread_t thread_q;
        pthread_create(&thread_q, NULL, (void*)contar_quantum, args);
        pthread_detach(thread_q);
    }
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


void contar_quantum(void *args) {
    struct arg_contar_quantum *ptr = args;
    struct arg_contar_quantum datos = *ptr;
    free(args);
    
    usleep(datos.quantum * 1000);
    if (tiempo_inicio_cuenta_q == datos.tiempo_inicio && exec != NULL) {
        interrumpir("QUANTUM");
    }
}


void desbloquear_proceso(int pid) {
    t_estado_proceso *est = buscar_estado_proceso(pid);
    t_pcb *pcb = buscar_pcb_por_pid(pid);
    if (config_kernel.algoritmo_planificacion != ALGO_VRR) {
        est->estado = ESTADO_READY;
        agregar_a_ready(pcb);
    }
    else {
        if (pcb->quantum > 0 && est->motivo_bloqueo == BLOQUEO_IO) {
            est->estado = ESTADO_READY_AUX;
            agregar_a_ready_aux(pcb);
        }
        else {
            est->estado = ESTADO_READY;
            agregar_a_ready(pcb);
        }
    }
    planificar();
}


void controlar_quantum(t_pcb *pcb) {
    if (config_kernel.algoritmo_planificacion != ALGO_VRR) return;
    int64_t tiempo_actual = temporal_gettime(temporal_global);
    int quantum_pasado = tiempo_actual - tiempo_inicio_cuenta_q;
    pcb->quantum = quantum_pasado;
}


void log_cola_ready(bool ready_aux) {
    t_log *logger = kernel_logger();
    t_queue *cola;
    if (ready_aux) {
        cola = cola_ready_aux;
    }
    else {
        cola = cola_ready;
    }
    int cant_proc_ready = queue_size(cola);
    char *lista_pid = string_new();
    for (int i = 0; i < cant_proc_ready; i++) {
        if (i > 0) {
            string_append(&lista_pid, ",");
        }
        t_pcb *pcb = list_get(cola->elements, i);
        char *pid_str = string_itoa(pcb->pid);
        string_append(&lista_pid, pid_str);

        free(pid_str);
    }
    if (ready_aux) {
        log_info(logger, "Cola Ready Prioridad: [%s]", lista_pid);
    }
    else {
        log_info(logger, "Cola Ready: [%s]", lista_pid);
    }
    
    free(lista_pid);
    log_destroy(logger);
}