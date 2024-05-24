#include "main.h"

struct timespec tiempo 

int main() {
    t_log *logger = log_create("io.log", "io", true, LOG_LEVEL_INFO);
    log_info(logger, "Iniciando I/O...");
    cargar_config();
    
    //cliente a memoria
    int conexion_memoria = crear_conexion(ip_memoria,puerto_memoria, "Hola soy el IO");
    //cliente a kernel
    int conexion_kernel = crear_conexion(ip_kernel,puerto_kernel, "Hola soy el IO");


    int server_fd = iniciar_servidor(config_valores.puerto);

    pthread_t io;
    pthread_create(&io,NULL,(void*)manejarIO,(void*)server_fd);
    pthread_detach(io);


    return 0;
    ///////////////////////////////////////////////////////////////////
}

void manejarIO(int server_fd){
    while(1){
        int socketCliente = esperar_cliente(server_fd);

        pthread_t t;
        pthread_create(&t,NULL,(void*)manejarConexion,(void*)socketCliente);
        pthread_detach(t);
    }
}

void manejarConexion(int socketCliente){
    int tipoInterfaz = recibir_operacion(socketCliente);

    switch(tipoInterfaz){
        case IO_GEN_SLEEP:
        iO_GEN_SLEEP(n);
    }
}