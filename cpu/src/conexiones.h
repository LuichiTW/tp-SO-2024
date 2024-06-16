#ifndef CONEXIONES_CPU_H
#define CONEXIONES_CPU_H

#include <utils/conexiones/conexiones_servidor.h>
#include <utils/conexiones/conexiones_cliente.h>
#include "config.h"

struct sockets_cpu {
    int socket_memoria;
    int socket_servidor_cpu_dispatch;
    int socket_servidor_cpu_interrupt;
    int socket_cliente_kernel_interrupt;
};

extern struct sockets_cpu sockets_cpu;

void cargar_sockets(t_log *loggerPrincipal);

#endif