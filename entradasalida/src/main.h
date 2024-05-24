#ifndef MAIN_IO
#define MAIN_IO

#include <stdlib.h>
#include <stdio.h>
#include <utils/conexiones/conexiones_cliente.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/error.h>
#include <pthread.h>
#include <readline/readline.h>
#include <utils/conexiones/conexiones_servidor.c>
#endif // !MAIN_IO


void iO_GEN_SLEEP(n){
    for (int i = 0; i < n; i++) {
        nanosleep(&tiempo, NULL);
    }
}

void cargar_config(void){
    config = config_create("../io.config");
    if (config == NULL) {
        log_error(logger, "No se leyo el archivo de configuracion");
        exit(EXIT_FAILURE);
    }
    char *tipo_interfaz = config_get_string_value(config,"TIPO_INTERFAZ");
    int *tiempo_unidad_trabajo = config_get_int_value(config,"TIEMPO_UNIDAD_TRABAJO");
    char *path_base_dialfs = config_get_string_value(config,"PATH_BASE_DIALFS");
    int *block_size = config_get_int_value(config,"BLOCK_SIZE");
    int *block_count = config_get_int_value(config,"BLOCK_COUNT");
    int *retraso_compactacion = config_get_int_value(config,"RETRASO_COMPACTACION");
    struct timespec tiempo = tiempo_unidad_trabajo;
    //cliente a memoria
    char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    //cliente a kernel
    char *ip_kernel = config_get_string_value(config, "IP_KERNEL");
    char *puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");

    struct timespec tiempo = tiempo_unidad_trabajo;
}