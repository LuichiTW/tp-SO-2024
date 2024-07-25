#include "config.h"

struct config_cpu config_cpu;
int tam_pagina;

void cargar_config() {
    t_config *config = config_create("cpu.config");

    if (config == NULL) {
        t_log *logger = log_create("cpu_alt.log", "cpu", true, LOG_LEVEL_INFO);
        log_error(logger, "No se leyo el archivo de configuracion");
        log_destroy(logger);
        exit(EXIT_FAILURE);
    }

    config_cpu.ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    config_cpu.puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    config_cpu.puerto_escucha_dispatch = config_get_string_value(config, "PUERTO_ESCUCHA_DISPATCH");
    config_cpu.puerto_escucha_interrupt = config_get_string_value(config, "PUERTO_ESCUCHA_INTERRUPT");
    config_cpu.cantidad_entradas_tlb = config_get_int_value(config, "CANTIDAD_ENTRADAS_TLB");
    config_cpu.algoritmo_tlb = config_get_string_value(config, "ALGORITMO_TLB");

    config_cpu.config_original = config;
}