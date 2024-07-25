#include "main.h"

char* PLACEHOLDER_PATH_SCRIPT = "prueba.txt";

int main() {

    t_log *logger = alt_memlogger();
	log_info(logger, "Iniciando Memoria...");

    cargar_config();
    iniciar_mem_usuario();
    
    int socket_server = iniciar_servidor(string_itoa(config_memoria.puerto_escucha));

    log_info(logger, "Memoria lista para recibir conexiones");

    procesos_actuales = list_create();
    tablas_paginas = list_create();

    // DEBUG
    crear_proceso(1, "prueba.txt");

    // ? Por ahora no veo motivos para crear el hilo
    // Crea un hilo que se encargue de esperar clientes para gestionar sus conexiones.
    //// pthread_t thread_receptor;
    //// pthread_create(&thread_receptor, NULL, (void*)recibir_conexiones, (void*)&socket_server);
    //// pthread_detach(thread_receptor);

    while (true) {
        t_log *logger = alt_memlogger();
        // TODO mandarle a la funcion handshake el modulo
        
        int socket_cliente = esperar_cliente2(socket_server, logger);
        hacer_handshake(socket_cliente);

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


void recibir_solicitudes(int *socket_cliente_dir) {
    int socket_cliente = *socket_cliente_dir;
    while (true) {
    int op = recibir_operacion(socket_cliente);

    t_log *logger = alt_memlogger();
    log_info(logger, "op_code: %i\n", op);

    t_list *datos;
    datos = recibir_paquete(socket_cliente);

    switch (op) {
        case MEM_CREAR_PROCESO:
            {
                int pid;
                char *scriptname;

                datos = recibir_paquete(socket_cliente);

                pid = *((int*) list_get(datos, 0));
                scriptname = list_get(datos, 1);

                // ! Devolver algo
                crear_proceso(pid, scriptname);
            }
            break;

        case MEM_ENVIAR_INSTRUCCION:
            {
                int pid;
                int n_instruccion;
                
                pid = *((int*) list_get(datos, 0));
                n_instruccion = *((int*) list_get(datos, 1));

                char *rta = enviar_instruccion(pid, n_instruccion);

                delay(config_memoria.retardo_respuesta);

                enviar_mensaje(rta, socket_cliente);
            }
            break;

        case MEM_FINALIZAR_PROCESO:
            {
                int pid;

                pid = *((int*) list_get(datos, 0));

                finalizar_proceso(pid);
            }
            break;
        
        case MEM_LEER_MEMORIA:
            {
                int dir_fisica;
                size_t tam;

                dir_fisica = *((int*) list_get(datos, 0));
                tam = *((size_t*) list_get(datos, 1));

                leer_memoria(dir_fisica, tam);
            }
            break;

        case MEM_ESCRIBIR_MEMORIA:
            {
                int dir_fisica;
                size_t tam;
                char *valor;

                dir_fisica = *((int*) list_get(datos, 0));
                tam = *((size_t*) list_get(datos, 1));
                valor = (char*) list_get(datos, 2);

                escribir_memoria(dir_fisica, tam, valor);
            }
            break;
            
        case MEM_RESIZE_PROCESO:
            {
                int pid;
                int nuevo_tam;

                pid = *((int*) list_get(datos, 0));
                nuevo_tam = *((int*) list_get(datos, 1));

                int rta = resize_proceso(pid, nuevo_tam);

                delay(config_memoria.retardo_respuesta);
                enviar_entero(rta, socket_cliente);
            }
            break;
        case MEM_ACCESO_TABLA_PAGINAS:
            {
                int pid;
                int pagina;

                pid = *((int*) list_get(datos, 0));
                pagina = *((int*) list_get(datos, 1));

                int rta = acceso_tabla_paginas(pid, pagina);
                enviar_entero(rta, socket_cliente);
            }
        default:
            break;
    }

    list_destroy(datos);

    print_frames_ocupados(0);
    }
}


void hacer_handshake(int socket_cliente) {
    recibir_operacion(socket_cliente);
    char *modulo_str = recibir_msg(socket_cliente);
    int i_modulo = atoi(modulo_str);

    switch (i_modulo) {
        case MOD_KERNEL:
            break;
        case MOD_CPU:
            // Responde a CPU con el tamaño de página
            enviar_mensaje(string_itoa(config_memoria.tam_pagina), socket_cliente);
            break;
        case MOD_IO:
            break;
    }
}


void delay(int milisegundos) {
    t_temporal *t = temporal_create();
    while(temporal_gettime(t) < milisegundos) {}
    temporal_destroy(t);
}