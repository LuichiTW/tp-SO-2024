#ifndef TLB_H
#define TLB_H

#include <stdlib.h>
#include <commons/collections/list.h>
#include <commons/temporal.h>
#include <commons/string.h>
#include "config.h"

typedef struct {
    int pid;
    int pagina;
    int frame;
    int64_t tiempo_carga; // Para FIFO
    int64_t tiempo_referencia; // Para LRU
} t_fila_tlb;

struct tlb {
    int cant_filas;
    t_list *lista;
    t_temporal *tiempo_inicio;
};

extern struct tlb tlb;


void inicializar_tlb();

// Devuelve el frame correspondiente a la página del proceso solicitada, o -1 si no la encuentra.
int tlb_obtener_frame(int pid, int pagina);

void tlb_agregar_fila(int pid, int pagina, int frame);

void tlb_reemplazar_fila_FIFO(t_fila_tlb *fila_nueva);
void tlb_reemplazar_fila_LRU(t_fila_tlb *fila_nueva);

#endif // TLB_H