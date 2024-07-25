#include "conexiones.h"

struct sockets_cpu sockets_cpu;

void cargar_sockets() {
    t_log *logger = log_create("cpu_extra.log", "cpu extra", true, LOG_LEVEL_INFO);

    // Memoria (cliente)
    log_info(logger, "Conectando a memoria...");
    sockets_cpu.socket_memoria = crear_conexion2(config_cpu.ip_memoria, config_cpu.puerto_memoria);
    log_info(logger, "Conexion creada\n");

    char modulo[5] = "    ";
    sprintf(modulo, "%d", MOD_CPU);
    enviar_mensaje(modulo, sockets_cpu.socket_memoria);

    recibir_operacion(sockets_cpu.socket_memoria);
    char *tam_pagina_str = recibir_msg(sockets_cpu.socket_memoria);
    log_info(logger, "Tam pag: %s", tam_pagina_str);
    tam_pagina = atoi(tam_pagina_str);

    //*SERVIDOR------------------------------------------>
        //*DISPATCH
    ////sockets_cpu.socket_servidor_cpu_dispatch = iniciar_servidor(config_cpu.puerto_escucha_dispatch);
	////log_info(logger, "Listo para recibir al Kernel en dispatch");
        //*INTERRUPT
    ////sockets_cpu.socket_servidor_cpu_interrupt = iniciar_servidor(config_cpu.puerto_escucha_interrupt);
	////log_info(logger, "Listo para recibir al Kernel en interrupt");
    log_destroy(logger);
}