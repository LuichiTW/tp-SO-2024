#include "conexion.h"

struct sockets sockets;

// TODO esperar conexiones de clientes IO

void levantar_conexiones() {
    // Memoria
    sockets.memoria = crear_conexion2(config_kernel.ip_memoria, config_kernel.puerto_memoria);
    enviar_entero(MOD_KERNEL, sockets.memoria);
    recibir_operacion(sockets.memoria);
    recibir_msg(sockets.memoria);
    
    // CPU
    sockets.cpu_dispatch = crear_conexion2(config_kernel.ip_cpu, config_kernel.puerto_cpu_dispatch);
    enviar_entero(MOD_KERNEL, sockets.cpu_dispatch);
    recibir_operacion(sockets.cpu_dispatch);
    recibir_msg(sockets.cpu_dispatch);

    sockets.cpu_interrupt = crear_conexion2(config_kernel.ip_cpu, config_kernel.puerto_cpu_interrupt);
    enviar_entero(MOD_KERNEL, sockets.cpu_interrupt);
    recibir_operacion(sockets.cpu_interrupt);
    recibir_msg(sockets.cpu_interrupt);

    // SERVER IO
    sockets.server_io = iniciar_servidor(config_kernel.puerto_escucha);
    sockets.clientes_io = list_create();
}