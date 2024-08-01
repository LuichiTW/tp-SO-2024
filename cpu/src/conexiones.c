#include "conexiones.h"

struct sockets_cpu sockets_cpu;
sem_t mutex_dispatch;

void cargar_sockets() {
    t_log *logger = log_create("cpu_extra.log", "cpu extra", true, LOG_LEVEL_INFO);

    // Memoria (cliente)
    log_info(logger, "Conectando a memoria...");
    sockets_cpu.socket_memoria = crear_conexion2(config_cpu.ip_memoria, config_cpu.puerto_memoria);
    log_info(logger, "Conexion creada");

    char modulo[5] = "    ";
    sprintf(modulo, "%d", MOD_CPU);
    enviar_mensaje(modulo, sockets_cpu.socket_memoria);

    recibir_operacion(sockets_cpu.socket_memoria);
    char *tam_pagina_str = recibir_msg(sockets_cpu.socket_memoria);
    log_info(logger, "Tam pag: %s", tam_pagina_str);
    tam_pagina = atoi(tam_pagina_str);

    // Levantar servidores para Kernel
    //* Dispatch

    log_info(logger, "Levantando servidor en puerto Dispatch...");
    sockets_cpu.socket_escucha_dispatch = iniciar_servidor(config_cpu.puerto_escucha_dispatch);
	log_info(logger, "Servidor listo");

    //* Interrupt

    log_info(logger, "Levantando servidor en puerto Interrupt...");
    sockets_cpu.socket_escucha_interrupt = iniciar_servidor(config_cpu.puerto_escucha_interrupt);
	log_info(logger, "Servidor listo");

    // Esperar conexiones del Kernel
    //* Dispatch

    sockets_cpu.socket_kernel_dispatch = esperar_cliente2(sockets_cpu.socket_escucha_dispatch, logger);
    recibir_operacion(sockets_cpu.socket_kernel_dispatch);
    int mod_dispatch = recibir_entero(sockets_cpu.socket_kernel_dispatch);
    if (mod_dispatch != MOD_KERNEL) exit(EXIT_FAILURE);
    enviar_entero(MOD_CPU, sockets_cpu.socket_kernel_dispatch);

    //* Interrupt

    sockets_cpu.socket_kernel_interrupt = esperar_cliente2(sockets_cpu.socket_escucha_interrupt, logger);
    recibir_operacion(sockets_cpu.socket_kernel_interrupt);
    int mod_interrupt = recibir_entero(sockets_cpu.socket_kernel_interrupt);
    if (mod_interrupt != MOD_KERNEL) exit(EXIT_FAILURE);
    enviar_entero(MOD_CPU, sockets_cpu.socket_kernel_interrupt);

    log_destroy(logger);

    sem_init(&mutex_dispatch, false, 1);
}