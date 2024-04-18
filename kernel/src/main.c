#include "main.h"

int main() {
    //Iniciar logger del kernel y su config
    t_log *logger = log_create("kernel.log", "kernel", true, LOG_LEVEL_INFO);
    log_info(logger, "Iniciando kernel...");
    t_config *config = config_create("../kernel.config");
    if (config == NULL) {
        log_error(logger, "No se leyo el archivo de configuracion");
        exit(EXIT_FAILURE);
    }

    //iniciar el cliente del kernel
    //memoria
    char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    int conexion_memoria = crear_conexion(ip_memoria, puerto_memoria, "Hola soy el kernel");
    
    //cpu
    char *puerto_cpu = config_get_string_value(config, "PUERTO_CPU_DISPATCH");
    char *ip_cpu = config_get_string_value(config, "IP_CPU");
    int conexion_cpu = crear_conexion(ip_cpu, puerto_cpu, "Hola soy el kernel");

    //iniciar el servidor del kernel
    //int conexion_io;
	char *puerto = config_get_string_value(config, "PUERTO_ESCUCHA");
    int socket_servidor_kernel = iniciar_servidor(puerto);
	log_info(logger, "Listo para recibir al IO");
    int socket_cliente_io = esperar_cliente(socket_servidor_kernel, logger);
    
    char* algo = readline("> ");
    //se tendria que liberar el especio de memoria usado por los elementos
    return 0;
}
