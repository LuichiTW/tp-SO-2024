#include "main.h"
int main() {
    t_log *logger = log_create("io.log", "io", true, LOG_LEVEL_INFO);
    log_info(logger, "Iniciando I/O...");
    t_config *config = config_create("../io.config");
    if (config == NULL) {
        log_error(logger, "No se leyo el archivo de configuracion");
        exit(EXIT_FAILURE);
    }

    //cliente a memoria
    char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    int conexion_memoria = crear_conexion(ip_memoria,puerto_memoria, "Hola soy el IO");
    //cliente a kernel
    char *ip_kernel = config_get_string_value(config, "IP_KERNEL");
    char *puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");
    int conexion_kernel = crear_conexion(ip_kernel,puerto_kernel, "Hola soy el IO");
    char* algo = readline("> ");
    return 0;
}
