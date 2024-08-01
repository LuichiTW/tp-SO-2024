#include "conexion.h"

struct sockets sockets;
sem_t mutex_dispatch;

// TODO esperar conexiones de clientes IO

void levantar_conexiones() {
    t_log *alt_logger = log_create("kernel_alt.log", "kernel", true, LOG_LEVEL_INFO);

    // Memoria
    log_info(alt_logger, "Conectando a Memoria...");
    sockets.memoria = crear_conexion2(config_kernel.ip_memoria, config_kernel.puerto_memoria);
    log_info(alt_logger, "Conexión creada. Empezando Handshake...");
    enviar_entero(MOD_KERNEL, sockets.memoria);
    recibir_operacion(sockets.memoria);
    int rta_mem = recibir_entero(sockets.memoria);
    if (rta_mem != MOD_MEMORIA) exit(EXIT_FAILURE);
    log_info(alt_logger, "Handshake exitoso");
    
    // CPU
    //* Dispatch
    log_info(alt_logger, "Conectando a CPU en el puerto Dispatch...");
    sockets.cpu_dispatch = crear_conexion2(config_kernel.ip_cpu, config_kernel.puerto_cpu_dispatch);

    log_info(alt_logger, "Conexión creada. Empezando Handshake...");
    enviar_entero(MOD_KERNEL, sockets.cpu_dispatch);

    recibir_operacion(sockets.cpu_dispatch);
    int rta_disp = recibir_entero(sockets.cpu_dispatch);
    if (rta_disp != MOD_CPU) exit(EXIT_FAILURE);
    log_info(alt_logger, "Handshake exitoso");

    //* Interrupt
    log_info(alt_logger, "Conectando a CPU en el puerto Dispatch...");
    sockets.cpu_interrupt = crear_conexion2(config_kernel.ip_cpu, config_kernel.puerto_cpu_interrupt);

    log_info(alt_logger, "Conexión creada. Empezando Handshake...");
    enviar_entero(MOD_KERNEL, sockets.cpu_interrupt);

    recibir_operacion(sockets.cpu_interrupt);
    int rta_interr = recibir_entero(sockets.cpu_interrupt);
    if (rta_interr != MOD_CPU) exit(EXIT_FAILURE);
    log_info(alt_logger, "Handshake exitoso");

    // SERVER IO
    sockets.server_io = iniciar_servidor(config_kernel.puerto_escucha);
    sockets.clientes_io = list_create();

    log_destroy(alt_logger);

    sem_init(&mutex_dispatch, false, 1);
}