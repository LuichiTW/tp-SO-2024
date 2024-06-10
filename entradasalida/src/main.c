#include "main.h"

int main() {
    t_log *logger = log_create("io.log", "io", true, LOG_LEVEL_INFO);
    log_info(logger, "Iniciando I/O...");
    t_config *config = config_create("./io.config");
    
    
    if (config == NULL) {
        log_error(logger, "No se leyo el archivo de configuracion");
        exit(EXIT_FAILURE);
    }
    char *tipo_interfaz = config_get_string_value(config,"TIPO_INTERFAZ");
    int tiempo_unidad_trabajo = config_get_int_value(config,"TIEMPO_UNIDAD_TRABAJO");
    char *path_base_dialfs = config_get_string_value(config,"PATH_BASE_DIALFS");
    int block_size = config_get_int_value(config,"BLOCK_SIZE");
    int block_count = config_get_int_value(config,"BLOCK_COUNT");
    int retraso_compactacion = config_get_int_value(config,"RETRASO_COMPACTACION");
    //memoria
    char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    //kernel
    char *ip_kernel = config_get_string_value(config, "IP_KERNEL");
    char *puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");


    tiempo.tv_sec = tiempo_unidad_trabajo;
    
    t_parametroEsperar parametros;

    
    //cliente a memoria
    parametros.conexion_memoria = crear_conexion(ip_memoria, puerto_memoria, "Hola soy el IO");
   

    //cliente a kernel
    parametros.conexion_kernel = crear_conexion(ip_kernel, puerto_kernel, "Hola soy el IO");
   
    //servidor

    parametros.server_fd = iniciar_servidor(puerto_kernel);
    parametros.logger = logger;

    pthread_t kernel_t;
    pthread_create(&kernel_t,NULL,(void*)esperar,(void*)&parametros);
    pthread_detach(kernel_t);


    return 0;
    ///////////////////////////////////////////////////////////////////
}

