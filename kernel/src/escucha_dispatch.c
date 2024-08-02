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
    if (string_equals_ignore_case(motivo[0], "INTERRUPT")) {
        if (string_equals_ignore_case(motivo[1], "FINALIZAR_PROCESO")) {
            t_log *logger = kernel_logger();
            log_info(logger, "Finaliza el proceso %i - Motivo: INTERRUPTED_BY_USER", exec->pid);
            log_destroy(logger);
            finalizar_proceso(exec);
        }
        planificar();
    }

    string_array_destroy(motivo);
}