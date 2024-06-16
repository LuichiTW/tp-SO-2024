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

    // ? Por ahora no veo motivos para crear el hilo
    // Crea un hilo que se encargue de esperar clientes para gestionar sus conexiones.
    //// pthread_t thread_receptor;
    //// pthread_create(&thread_receptor, NULL, (void*)recibir_conexiones, (void*)&socket_server);
    //// pthread_detach(thread_receptor);

    while (true) {
        t_log *logger = alt_memlogger();
        int socket_cliente = esperar_cliente(socket_server, logger);

        // Cuando un cliente se conecta, crea un hilo para atender sus peticiones.
        pthread_t thread_conexion;
        pthread_create(&thread_conexion, NULL, (void*)recibir_solicitudes, (void*)&socket_cliente);
        pthread_detach(thread_conexion);
        
        log_destroy(logger);
    }  
    
    ////list_destroy_and_destroy_elements(procesos_actuales);
    config_destroy(config_memoria.config);
    return 0;
}


void recibir_solicitudes(int socket_cliente) {
    int op = recibir_operacion(socket_cliente);

    t_list *datos;
    datos = recibir_paquete(socket_cliente);

    switch (op) {
        case MEM_CREAR_PROCESO:
            {
                int pid;
                char *scriptname;

                datos = recibir_paquete(socket_cliente);

                pid = (int) list_get(datos, 0);
                scriptname = list_get(datos, 1);

                // ! Devolver algo
                int rta = crear_proceso(pid, scriptname);
            }
            break;

        case MEM_ENVIAR_INSTRUCCION:
            {
                int pid;
                uint n_instruccion;
                
                pid = (int) list_get(datos, 0);
                n_instruccion = (uint) list_get(datos, 1);

                enviar_instruccion(pid, n_instruccion);
            }
            break;

        case MEM_FINALIZAR_PROCESO:
            {
                int pid;

                pid = (int) list_get(datos, 0);

                finalizar_proceso(pid);
            }
            break;
        
        case MEM_LEER_MEMORIA:
            {
                int dir_fisica;
                size_t tam;

                dir_fisica = (int) list_get(datos, 0);
                tam = (size_t) list_get(datos, 1);

                leer_memoria(dir_fisica, tam);
            }
            break;

        case MEM_ESCRIBIR_MEMORIA:
            {
                int dir_fisica;
                size_t tam;
                char *valor;

                dir_fisica = (int) list_get(datos, 0);
                tam = (size_t) list_get(datos, 1);
                valor = (char*) list_get(datos, 2);

                escribir_memoria(dir_fisica, tam, valor);
            }
            break;
            
        case MEM_RESIZE_PROCESO:
            {
                int pid;
                uint nuevo_tam;

                pid = (int) list_get(datos, 0);
                nuevo_tam = (uint) list_get(datos, 1);

                resize_proceso(pid, nuevo_tam);
            }
            break;
        case MEM_ACCESO_TABLA_PAGINAS:
            {
                int pid;
                int pagina;

                pid = (int) list_get(datos, 0);
                pagina = (int) list_get(datos, 1);

                acceso_tabla_paginas(pid, pagina);
            }
        default:
            break;
    }

    list_destroy(datos);

    print_frames_ocupados(0);
}