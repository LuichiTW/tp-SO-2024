#include "consola.h"

void iniciar_consola() {
    t_log *alt_logger = log_create("kernel_alt.log", "kernel", true, LOG_LEVEL_INFO);
    log_debug(alt_logger, "Consola interactiva iniciada.");
    
    while (true) {

        char *comando_str = readline("> ");
        if (string_length(comando_str) == 0) continue;

        char **comando_dividido = string_split(comando_str, " ");
        int comando = traducir_comando(comando_dividido[0]);

        if (comando == -1) {
            log_error(alt_logger, "Comando ingresado no válido.");
            continue;
        }


        switch (comando) {
        case COM_EJECUTAR_SCRIPT:
            if (string_array_size(comando_dividido) < 2) {
                log_error(alt_logger, "Parámetros faltantes");
                continue;
            }
            c_ejecutar_script(comando_dividido[1]);
            break;
        case COM_INICIAR_PROCESO:
            if (string_array_size(comando_dividido) < 2) {
                log_error(alt_logger, "Parámetros faltantes");
                continue;
            }
            c_iniciar_proceso(comando_dividido[1]);
            break;
        case COM_FINALIZAR_PROCESO:
            if (string_array_size(comando_dividido) < 2) {
                log_error(alt_logger, "Parámetros faltantes");
                continue;
            }
            c_finalizar_proceso(atoi(comando_dividido[1]));
            break;
        case COM_DETENER_PLANIFICACION:
            c_detener_planificacion();
            break;
        case COM_INICIAR_PLANIFICACION:
            c_iniciar_planificacion();
            break;
        case COM_MULTIPROGRAMACION:
            if (string_array_size(comando_dividido) < 2) {
                log_error(alt_logger, "Parámetros faltantes");
                continue;
            }
            c_multiprogramacion(atoi(comando_dividido[1]));
            break;
        case COM_PROCESO_ESTADO:
            c_proceso_estado();
            break;
        }

        free(comando_str);
        string_array_destroy(comando_dividido);
    }
    log_destroy(alt_logger);
}

int traducir_comando(char *comando) {
    if (string_equals_ignore_case(comando, "EJECUTAR_SCRIPT")) return COM_EJECUTAR_SCRIPT;
    if (string_equals_ignore_case(comando, "INICIAR_PROCESO")) return COM_INICIAR_PROCESO;
    if (string_equals_ignore_case(comando, "FINALIZAR_PROCESO")) return COM_FINALIZAR_PROCESO;
    if (string_equals_ignore_case(comando, "DETENER_PLANIFICACION")) return COM_DETENER_PLANIFICACION;
    if (string_equals_ignore_case(comando, "INICIAR_PLANIFICACION")) return COM_INICIAR_PLANIFICACION;
    if (string_equals_ignore_case(comando, "MULTIPROGRAMACION")) return COM_MULTIPROGRAMACION;
    if (string_equals_ignore_case(comando, "PROCESO_ESTADO")) return COM_PROCESO_ESTADO;
    return -1;
}

void c_ejecutar_script(char *path) {

}
void c_iniciar_proceso(char *path) {
    t_pcb *pcb = crear_proceso();

    t_paquete *paquete = crear_paquete();
    agregar_a_paquete(paquete, &(pcb->pid), sizeof(pcb->pid));
    agregar_a_paquete(paquete, path, string_length(path));
    enviar_peticion(paquete, sockets.memoria, MEM_CREAR_PROCESO);
    eliminar_paquete(paquete);

    recibir_operacion(sockets.memoria);
    recibir_entero(sockets.memoria);

    queue_push(cola_new, pcb);
    planificar();
}
void c_finalizar_proceso(int pid) {

}
void c_detener_planificacion() {

}
void c_iniciar_planificacion() {

}
void c_multiprogramacion(int valor) {

}
void c_proceso_estado() {

}