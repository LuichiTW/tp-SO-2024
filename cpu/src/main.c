#include "main.h"
//variable global de los registros del CPU, se usa el Keyword extern para poder hacer uso de estos registros.
registros_cpu regcpu;
//log global del CPU
t_log *loggerPrincipal;
int main() {
    loggerPrincipal = log_create("cpu.log", "cpu", true, LOG_LEVEL_INFO);
	log_info(loggerPrincipal, "Iniciando CPU...");
    t_config *config = config_create("./cpu.config");
    if (config == NULL)
    {
        log_error(loggerPrincipal, "No se leyo el archivo de configuracion");
        exit(EXIT_FAILURE);
    }

//se omite la conexiones entre modulos para hacer pruebas independientemente en el modulo

/*     char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    int conexion_memoria = crear_conexion(ip_memoria,puerto_memoria,"Hola soy el CPU");

	char *puerto_dispatch = config_get_string_value(config, "PUERTO_ESCUCHA_DISPATCH");
    int socket_servidor_cpu = iniciar_servidor(puerto_dispatch);
	log_info(loggerPrincipal, "Listo para recibir al Kernel");
    int socket_cliente_kernel = esperar_cliente(socket_servidor_cpu, loggerPrincipal);
 */

    char test=readline("> ");
    return 0;
}
