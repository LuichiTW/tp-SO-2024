#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>

struct config_cpu {
    char* ip_memoria;
    int puerto_memoria;
    int puerto_escucha_dispatch;
    int puerto_escucha_interrupt;
    int cantidad_entradas_tlb;
    char* algoritmo_tlb;
    t_config config_original;
};

extern struct config_cpu config_cpu;

void cargar_config(void);