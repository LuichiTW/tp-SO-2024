#ifndef CONEXION_KERNEL_H
#define CONEXION_KERNEL_H

#include <semaphore.h>
#include <commons/collections/list.h>
#include <commons/log.h>
#include <utils/conexiones/conexiones_cliente.h>
#include <utils/conexiones/conexiones_servidor.h>
#include "config.h"

struct sockets {
    int memoria;
    int cpu_dispatch;
    int cpu_interrupt;
    int server_io;
    t_list *clientes_io;
};

extern struct sockets sockets;
extern sem_t mutex_dispatch;

void levantar_conexiones();

#endif // CONEXION_KERNEL_H