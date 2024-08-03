#ifndef CONFIG_KERNEL_H
#define CONFIG_KERNEL_H

#include <stdlib.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/string.h>

enum algoritmo_planificacion {
    ALGO_FIFO,
    ALGO_RR,
    ALGO_VRR
};

struct config_kernel {
    char *puerto_escucha;
    char *ip_memoria;
    char *puerto_memoria;
    char *ip_cpu;
    char *puerto_cpu_dispatch;
    char *puerto_cpu_interrupt;
    enum algoritmo_planificacion algoritmo_planificacion;
    int quantum;
    // ? recursos
    // ? instancias_recursos
    int grado_multiprogramacion;
    t_config *config_original;
};

extern struct config_kernel config_kernel;
extern int multiprogramacion_actual;

t_log *kernel_logger();
t_log *alt_kernel_logger();

void cargar_config();

#endif // CONFIG_KERNEL_H