#include "conexion.h"

struct sockets sockets;
sem_t mutex_dispatch;

// TODO esperar conexiones de clientes IO

void levantar_conexiones() {
    // Memoria
    sockets.memoria = crear_conexion2(config_kernel.ip_memoria, config_kernel.puerto_memoria);
    enviar_entero(MOD_KERNEL, sockets.memoria);
    recibir_operacion(sockets.memoria);
    int rta_mem = recibir_entero(sockets.memoria);
    if (rta_mem != MOD_MEMORIA) exit(EXIT_FAILURE);
    
    // CPU
    //* Dispatch
    sockets.cpu_dispatch = crear_conexion2(config_kernel.ip_cpu, config_kernel.puerto_cpu_dispatch);

    enviar_entero(MOD_KERNEL, sockets.cpu_dispatch);

    recibir_operacion(sockets.cpu_dispatch);
    int rta_disp = recibir_entero(sockets.cpu_dispatch);
    if (rta_disp != MOD_CPU) exit(EXIT_FAILURE);

    //* Interrupt
    sockets.cpu_interrupt = crear_conexion2(config_kernel.ip_cpu, config_kernel.puerto_cpu_interrupt);

    enviar_entero(MOD_KERNEL, sockets.cpu_interrupt);

    recibir_operacion(sockets.cpu_interrupt);
    int rta_interr = recibir_entero(sockets.cpu_interrupt);
    if (rta_interr != MOD_CPU) exit(EXIT_FAILURE);

    sem_init(&mutex_dispatch, false, 1);
}