#include "config.h"

struct config_kernel config_kernel;
int multiprogramacion_actual = 0;

// TODO listas recursos

void cargar_config() {
    t_config *config = config_create("kernel.config");

    if (config == NULL) {
        t_log *logger = alt_kernel_logger();
        log_error(logger, "No se leyó el archivo de configuracion");
        log_destroy(logger);
        exit(EXIT_FAILURE);
    }

    config_kernel.puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
    config_kernel.ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    config_kernel.puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    config_kernel.ip_cpu = config_get_string_value(config, "IP_CPU");
    config_kernel.puerto_cpu_dispatch = config_get_string_value(config, "PUERTO_CPU_DISPATCH");
    config_kernel.puerto_cpu_interrupt = config_get_string_value(config, "PUERTO_CPU_INTERRUPT");
    config_kernel.algoritmo_planificacion = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
    config_kernel.quantum = config_get_int_value(config, "QUANTUM");
    // ? recursos
    // ? instancias_recursos
    config_kernel.grado_multiprogramacion = config_get_int_value(config, "GRADO_MULTIPROGRAMACION");
    
    config_kernel.config_original = config;
}


t_log *kernel_logger() {
    return log_create("kernel.log", "kernel", false, LOG_LEVEL_INFO);
}

t_log *alt_kernel_logger() {
    return log_create("kernel_extra.log", "kernel extra", false, LOG_LEVEL_INFO);
}