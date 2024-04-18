#include "main.h"

int main() {
    t_log *logger = log_create("memoria.log", "memoria", true, LOG_LEVEL_INFO);
	log_info(logger, "Iniciando Memoria...");
    t_config *config = config_create("../memory.config");
    if (config == NULL)
    {
        log_error(logger, "No se leyo el archivo de configuracion");
        exit(EXIT_FAILURE);
    }
	char *puerto = config_get_string_value(config, "PUERTO_ESCUCHA");

    int socket_servidor_memoria = iniciar_servidor(puerto);
	log_info(logger, "Listo para recibir al CPU");
    int socket_cliente_cpu = esperar_cliente(socket_servidor_memoria, logger);
    
	log_info(logger, "Listo para recibir al Kernel");
    int socket_cliente_kernel = esperar_cliente(socket_servidor_memoria, logger);
    
	log_info(logger, "Listo para recibir al IO");
    int socket_cliente_io = esperar_cliente(socket_servidor_memoria, logger);

    char* algo = readline("> ");
    return 0;
}
