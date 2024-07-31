#ifndef CONFIG_KERNEL_H
#define CONFIG_KERNEL_H

#include <stdlib.h>
#include <commons/config.h>
#include <commons/log.h>

struct config_kernel {
    char *puerto_escucha;
    char *ip_memoria;
    char *puerto_memoria;
    char *ip_cpu;
    char *puerto_cpu_dispatch;
    char *puerto_cpu_interrupt;
    char *algoritmo_planificacion;
    int quantum;
    // ? recursos
    // ? instancias_recursos
    int grado_multiprogramacion;
    t_config *config_original;
};

extern struct config_kernel config_kernel;
extern int multiprogramacion_actual;

void cargar_config();

#endif // CONFIG_KERNEL_H