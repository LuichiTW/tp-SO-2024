#ifndef CONEXIONES_CPU_H
#define CONEXIONES_CPU_H

#include <utils/conexiones/conexiones_servidor.h>
#include <utils/conexiones/conexiones_cliente.h>
#include "config.h"

struct sockets_cpu {
    int socket_memoria;
    int socket_kernel_interrupt;
    int socket_kernel_dispatch;
};

extern struct sockets_cpu sockets_cpu;

void cargar_sockets();

#endif