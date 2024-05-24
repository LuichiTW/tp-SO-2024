#include "memoria.h"

void *mem_usuario;
t_config_memoria config_memoria;



void cargar_config() {
    t_log *logger = crear_memlogger();
    log_info(logger, "Cargando configuración del módulo memoria...");

    t_config *config = config_create("../memory.config");

    if (config == NULL) {
        log_error(logger, "Ocurrió un error al cargar el archivo de configuración");
        exit(EXIT_FAILURE);
    }

    config_memoria.puerto_escucha = config_get_int_value(config, "PUERTO_ESCUCHA");
    config_memoria.tam_memoria = config_get_int_value(config, "TAM_MEMORIA");
    config_memoria.tam_pagina = config_get_int_value(config, "TAM_PAGINA");
    config_memoria.path_instrucciones = config_get_string_value(config, "PATH_INSTRUCCIONES");
    config_memoria.retardo_respuesta = config_get_int_value(config, "RETARDO_RESPUESTA");
    
    //! Debería comprobar que cargó cada cosa bien

    log_info(logger, "Configuración del módulo memoria cargada con éxito");

    log_destroy(logger);
    config_destroy(config);
}



void iniciar_mem_usuario() {
    t_log *logger = crear_memlogger();
    log_info(logger, "Asignando espacio de memoria de usuario...");
    mem_usuario = malloc(config_memoria.tam_memoria);

    if (mem_usuario == NULL) {
        log_error(logger, "No hay espacio suficiente en memoria");
    }

    log_info(logger, "Memoria asignada con éxito");
    log_destroy(logger);
}



t_log *crear_memlogger() {
    return log_create("memoria.log", "memoria", true, LOG_LEVEL_INFO);
}