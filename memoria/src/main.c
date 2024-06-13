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

    //mem_hexdump(mem_usuario, config_memoria.tam_memoria);

    // Crea un hilo que se encargue de esperar clientes para gestionar sus conexiones.

    // DEBUG (descomentar)
    /*
    pthread_t thread_receptor;
    pthread_create(&thread_receptor, NULL, recibir_conexiones, (void*)socket_server);
    pthread_detach(thread_receptor);
    */

   // DEBUG
   recibir_solicitudes(1);
   // DEBUG
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
        //int op = recibir_operacion(socket_cliente);

        // DEBUG (todo lo relacionado con readline)

        char* input;
        input = readline("> ");

        printf("Codigo de operacion:\n");
        int op = strtol(input, NULL, 10);
        printf("%i\n", op);

        int pid;

        switch (op) {
            case CREAR_PROCESO:
                char *scriptname;

                // DEBUG
                char *input;
                printf("PID:\n");
                input = readline("> ");
                printf("%s\n", input);
                pid = strtol(input, NULL, 10);

                printf("SCRIPT NAME:\n");
                scriptname = readline("> ");
                printf("%s\n", scriptname);
                // DEBUG

                crear_proceso(pid, scriptname);
                break;

            case ENVIAR_INSTRUCCION:
                uint n_instruccion;

                // DEBUG
                printf("PID:\n");
                input = readline("> ");
                printf("%s\n", input);
                pid = strtol(input, NULL, 10);

                printf("Num Instr:\n");
                input = readline("> ");
                printf("%s\n", input);
                n_instruccion = strtol(input, NULL, 10);
                // DEBUG

                enviar_instruccion(pid, n_instruccion);
                break;

            case FINALIZAR_PROCESO:
                // DEBUG
                printf("PID:\n");
                input = readline("> ");
                printf("%s\n", input);
                pid = strtol(input, NULL, 10);
                // DEBUG
                finalizar_proceso(pid);
                break;
                
            case RESIZE_PROCESO:
                uint nuevo_tam;

                // DEBUG
                printf("PID:\n");
                input = readline("> ");
                printf("%s\n", input);
                pid = strtol(input, NULL, 10);

                printf("Nuevo tam:\n");
                input = readline("> ");
                printf("%s\n", input);
                nuevo_tam = strtol(input, NULL, 10);
                // DEBUG

                resize_proceso(pid, nuevo_tam);
                break;

            default:
                break;
        }
    }
    
    return NULL;
}