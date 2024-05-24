#include "main.h"

int main() {

    t_log *logger = log_create("memoria.log", "memoria", true, LOG_LEVEL_INFO);

	log_info(logger, "Iniciando Memoria...");

    cargar_config();
    iniciar_mem_usuario();

	char puerto[5];
    sprintf(puerto, "%d", config_memoria.puerto_escucha);

    int socket_servidor_memoria = iniciar_servidor(puerto);
	log_info(logger, "Listo para recibir al CPU");
    int socket_cliente_cpu = esperar_cliente(socket_servidor_memoria, logger);
    
	log_info(logger, "Listo para recibir al Kernel");
    int socket_cliente_kernel = esperar_cliente(socket_servidor_memoria, logger);
    
	log_info(logger, "Listo para recibir al IO");
    int socket_cliente_io = esperar_cliente(socket_servidor_memoria, logger);

    printf("%i\n%i\n%i\n%i\n", socket_servidor_memoria, socket_cliente_cpu, socket_cliente_kernel, socket_cliente_io);

    //char* algo = readline("> ");
    return 0;
}
