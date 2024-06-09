#include "main.h"

char* PLACEHOLDER_PATH_SCRIPT = "prueba.txt";

int main() {

    t_log *logger = crear_memlogger();
	log_info(logger, "Iniciando Memoria...");

    cargar_config();
    iniciar_mem_usuario();
    
    char puerto[5];
    sprintf(puerto, "%d", config_memoria.puerto_escucha);
    int socket_server = iniciar_servidor(puerto);

    log_info(logger, "Memoria lista para recibir conexiones");

    procesos_actuales = list_create();
    tablas_paginas = list_create();

    //Testeo
    crear_proceso(1);
    enviar_instruccion(1);

    // Crea un hilo que se encargue de esperar clientes para gestionar sus conexiones.
    pthread_t thread_receptor;
    pthread_create(&thread_receptor, NULL, recibir_conexiones, (void*)socket_server);
    pthread_detach(thread_receptor);

    while (true) {}    
    
    //list_destroy_and_destroy_elements(procesos_actuales);
    config_destroy(config_memoria.config);
    return 0;
}



void *recibir_conexiones(int socket_server) {
    t_log *logger = crear_memlogger();
    
    // Espera constantemente a que se conecte un cliente.
    while (true) {
        int socket_cliente = esperar_cliente(socket_server, logger);

        // Cuando un cliente se conecta, crea un hilo para atender sus peticiones.
        pthread_t thread_conexion;
        pthread_create(&thread_conexion, NULL, recibir_solicitudes, (void*)socket_cliente);
        pthread_detach(thread_conexion);
    }

    return NULL;
}

void *recibir_solicitudes(int socket_cliente) {
    while (true) {
        int op = recibir_operacion(socket_cliente);

        switch (op) {
        case CREAR_PROCESO:
            crear_proceso(socket_cliente);
            break;
        case ENVIAR_INSTRUCCION:
            enviar_instruccion(socket_cliente);
            break;
        default:
            break;
        }
    }
    
    return NULL;
}