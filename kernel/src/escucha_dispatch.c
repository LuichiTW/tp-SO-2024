#include "escucha_dispatch.h"

void atender_dispatch() {
    while (true) {
        sem_wait(&mutex_dispatch);
        int op = recibir_operacion(sockets.cpu_dispatch);

        if (op == KER_CDE) {
            t_list *cde = recibir_paquete(sockets.cpu_dispatch);
            
            char *motivo_desalojo = list_remove(cde, list_size(cde) - 1);
            t_pcb *pcb_actualizado = desempaquetar_pcb(cde);

            exec->pid = pcb_actualizado->pid;
            exec->pc = pcb_actualizado->pc;
            exec->quantum = pcb_actualizado->quantum;
            exec->registros = pcb_actualizado->registros;

            manejar_desalojo(motivo_desalojo);
            
            list_destroy(cde);
            free(pcb_actualizado);
            free(motivo_desalojo);
        }

        sem_post(&mutex_dispatch);
    }
    
}

void manejar_desalojo(char *motivo_raw) {
    char **motivo = string_split(motivo_raw, " ");

    if (string_equals_ignore_case(motivo[0], "EXIT")) {
        t_log *logger = kernel_logger();
        log_info(logger, "Finaliza el proceso %i - Motivo: SUCCESS", exec->pid);
        log_destroy(logger);
        finalizar_proceso(exec);
        planificar();
    }
    if (string_equals_ignore_case(motivo[0], "OUT_OF_MEMORY")) {
        t_log *logger = kernel_logger();
        log_info(logger, "Finaliza el proceso %i - Motivo: OUT_OF_MEMORY", exec->pid);
        log_destroy(logger);
        finalizar_proceso(exec);
        planificar();
    }
    if (string_equals_ignore_case(motivo[0], "INTERRUPT")) {
        if (string_equals_ignore_case(motivo[1], "FINALIZAR_PROCESO")) {
            t_log *logger = kernel_logger();
            log_info(logger, "Finaliza el proceso %i - Motivo: INTERRUPTED_BY_USER", exec->pid);
            log_destroy(logger);
            finalizar_proceso(exec);
        }
        if (string_equals_ignore_case(motivo[1], "QUANTUM")) {
            t_log *logger = kernel_logger();
            log_info(logger, "PID: %i - Desalojado por fin de Quantum", exec->pid);
            log_destroy(logger);

            exec->quantum = 0;
            agregar_a_ready(exec);
            log_cola_ready(false);
            exec = NULL;
        }
        planificar();
    }
    if (string_equals_ignore_case(motivo[0], "IO_GEN_SLEEP")) {
        op_code op = IO_GEN_SLEEP;
        intentar_peticion_io(motivo[1], exec->pid, op, motivo[2]);
        exec = NULL;
    }

    string_array_destroy(motivo);
}