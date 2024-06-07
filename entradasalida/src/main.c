#include "main.h"

struct timespec tiempo;

int main() {
    t_log *logger = log_create("io.log", "io", true, LOG_LEVEL_INFO);
    log_info(logger, "Iniciando I/O...");
    t_config *config = config_create("../io.config");
    cargar_config();
    

    tiempo = tiempo_unidad_trabajo;
    
    //cliente a memoria
    int conexion_memoria = crear_conexion(ip_memoria,puerto_memoria, "Hola soy el IO");
    //cliente a kernel
    int conexion_kernel = crear_conexion(ip_kernel,puerto_kernel, "Hola soy el IO");


    int server_fd = iniciar_servidor(puerto_kernel);

    pthread_t conexion_kernel;
    pthread_create(&conexion_kernel,NULL,(void*)esperar,(void*)server_fd);
    pthread_detach(conexion_kernel);


    return 0;
    ///////////////////////////////////////////////////////////////////
}

void esperar(int server_fd){
    while(1){
        int socketCliente = esperar_cliente(server_fd,logger);

        pthread_t t;
        pthread_create(&t,NULL,(void*)manejarConexion,(void*)socketCliente);
        pthread_detach(t);
    }
}

void manejarConexion(int socketCliente){
    int tipoInterfaz = recibir_operacion(socketCliente);

    switch(tipoInterfaz){
        case IO_GEN_SLEEP:
        iO_GEN_SLEEP(socketCliente);
        break;
        case IO_STDIN_READ:
        iO_STDIN_READ(socketCliente);
        break;
        case IO_STDOUT_WRITE:
        iO_STDOUT_WRITE(socketCliente);
        break;
    }
}

void cargar_config(){
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
    //memoria
    char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    //kernel
    char *ip_kernel = config_get_string_value(config, "IP_KERNEL");
    char *puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");
}

void iO_GEN_SLEEP(socketCliente){
    int size;
    char * buffer;
    int desp = 0;

    buffer = recibir_buffer(&size,socketCliente);

    int uTrabajo = leer_entero(buffer,&desp);
    for (int i = 0; i < uTrabajo; i++) {
        nanosleep(&tiempo, NULL);
    }
    free(buffer);
}

void iO_STDIN_READ(socketCliente){
    int size;
    char * buffer;
    int desp = 0;
    char* texto;

    buffer = recibir_buffer(&size,socketCliente);
    uint64_t direcciones[] = leer_array(buffer,&desp);
    size_t num_direcciones = sizeof(direcciones) / sizeof(direcciones[0]);

    texto = readline("> ");

    for (size_t i = 0; i < num_direcciones; ++i){
        char *direccion = (char*)direcciones[i];
        strncpy(direccion, texto, &size);
    }
    free(texto);
    free(buffer);
    free(direcciones);
}

void iO_STDOUT_WRITE(socketCliente){
    int size;
    char * buffer;
    int desp = 0;
    char* texto;

    buffer = recibir_buffer(&size,socketCliente);
    uint64_t direcciones[] = leer_array(buffer,&desp);
    size_t num_direcciones = sizeof(direcciones) / sizeof(direcciones[0]);

    for (size_t i = 0; i < num_direcciones; ++i){
        char *direccion = (char*)direcciones[i];
        strncpy(texto, direccion, &size);
        printf("Texto leido: %c\n", texto);
    }
    free(texto);
    free(buffer);
    free(direcciones);
}