#include "memoria.h"

void *mem_usuario;
t_bitarray *frames_ocupados;
t_config_memoria config_memoria;
t_list *procesos_actuales;
t_list *tablas_paginas;
u_int cant_frames;


void cargar_config() {
    t_log *logger = alt_memlogger();
    log_info(logger, "Cargando configuración del módulo memoria...");

    t_config *config = config_create("memory.config");

    if (config == NULL) {
        log_error(logger, "Ocurrió un error al cargar el archivo de configuración");
        exit(EXIT_FAILURE);
    }

    config_memoria.puerto_escucha = config_get_int_value(config, "PUERTO_ESCUCHA");
    config_memoria.tam_memoria = config_get_int_value(config, "TAM_MEMORIA");
    config_memoria.tam_pagina = config_get_int_value(config, "TAM_PAGINA");
    config_memoria.path_instrucciones = config_get_string_value(config, "PATH_INSTRUCCIONES");
    config_memoria.retardo_respuesta = config_get_int_value(config, "RETARDO_RESPUESTA");

    config_memoria.config = config;
    
    //! Debería comprobar que cargó cada cosa bien

    log_info(logger, "Configuración del módulo memoria cargada con éxito");

    log_destroy(logger);
}



void iniciar_mem_usuario() {
    t_log *logger = alt_memlogger();
    log_info(logger, "Asignando espacio de memoria de usuario...");
    mem_usuario = malloc(config_memoria.tam_memoria);

    if (mem_usuario == NULL) {
        log_error(logger, "No hay espacio suficiente en memoria");
    }

    log_info(logger, "Memoria asignada con éxito");

    iniciar_bitmap_frames_ocupados();

    log_destroy(logger);
}


void iniciar_bitmap_frames_ocupados() {
    cant_frames = config_memoria.tam_memoria / config_memoria.tam_pagina;

    int cant_frames_en_bytes = ceil(cant_frames / 8);
    void *espacio_frames = calloc(cant_frames_en_bytes, sizeof(char)); // Divido por 8 porque necesito un bit por frame (no un byte).

    frames_ocupados = bitarray_create_with_mode(espacio_frames, cant_frames_en_bytes, LSB_FIRST);
}


t_tabla_paginas *crear_tabla_paginas(int pid) {
    t_tabla_paginas *tabla;
    tabla = malloc(sizeof(tabla));
    tabla->pid = pid;
    tabla->paginas = list_create();
    tabla->cant = 0;
    return tabla;
}


u_int cant_frames_libres() {
    u_int libres = 0;
    for (int i = 0; i < cant_frames; i++) {
        if (!bitarray_test_bit(frames_ocupados, i)) libres++;
    }

    return libres;
}

int prox_frame_libre() {
    for (int i = 0; i < cant_frames; i++) {
        if (!bitarray_test_bit(frames_ocupados, i)) return i;
    }

    return -1;
}

t_tabla_paginas *obtener_tabla_por_pid(int pid) {
    int es_el_buscado(t_tabla_paginas* tabla) {
        return tabla->pid == pid;
    }

    return list_find(tablas_paginas, (void*) es_el_buscado);
}



t_log *crear_memlogger() {
    return log_create("memoria.log", "memoria", true, LOG_LEVEL_INFO);
}

t_log *alt_memlogger() {
    return log_create("memoria_extra.log", "memoria", true, LOG_LEVEL_INFO);
}