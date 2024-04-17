#include "main.h"

int main() {
    t_log *logger = log_create("cpu.log", "cpu", true, LOG_LEVEL_INFO);
	log_info(logger, "Iniciando CPU...");
    t_config *config = config_create("cpu.config");
    if (config == NULL)
    {
        log_error(logger, "No se leyo el archivo de configuracion");
        exit(EXIT_FAILURE);
    }
    char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    int conexion_memoria = crear_conexion(ip_memoria,puerto_memoria,"Hola soy el CPU");

    //int conexion;
	char *puerto = config_get_string_value(config, "PUERTO_ESCUCHA_DISPATCH");
    int socket_servidor_cpu = iniciar_servidor(puerto);
	log_info(logger, "Listo para recibir al Kernel");
    int socket_cliente_kernel = esperar_cliente(socket_servidor_cpu, logger);

    return 0;
}
