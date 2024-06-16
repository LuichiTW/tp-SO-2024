#include "conexiones.h"

struct sockets_cpu sockets_cpu;

void cargar_sockets(t_log *loggerPrincipal) {
    //* CLIENTE------------------------------------------>
    sockets_cpu.socket_memoria = crear_conexion(config_cpu.ip_memoria, config_cpu.puerto_memoria, "Hola soy el CPU");

    //*SERVIDOR------------------------------------------>
        //*DISPATCH
    sockets_cpu.socket_servidor_cpu_dispatch = iniciar_servidor(config_cpu.puerto_escucha_dispatch);
	log_info(loggerPrincipal, "Listo para recibir al Kernel en dispatch");
        //*INTERRUPT
    sockets_cpu.socket_servidor_cpu_interrupt = iniciar_servidor(config_cpu.puerto_escucha_interrupt);
	log_info(loggerPrincipal, "Listo para recibir al Kernel en interrupt");
}