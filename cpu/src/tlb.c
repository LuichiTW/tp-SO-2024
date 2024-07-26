#include "tlb.h"

struct tlb tlb;

void inicializar_tlb() {
    tlb.cant_filas = 0;
    tlb.lista = list_create();
    tlb.tiempo_inicio = temporal_create();
}

int tlb_obtener_frame (int pid, int pagina) {
    int es_el_buscado(t_fila_tlb* fila) {
        printf("pid: %i\n", pid);
        printf("fila->pid: %i\n", fila->pid);
        printf("pag: %i\n", pagina);
        printf("fila->pag: %i\n", fila->pagina);
        return fila->pid == pid && fila->pagina == pagina;
    }

    t_log *logger = log_create("cpu.log", "cpu", true, LOG_LEVEL_INFO);

    t_fila_tlb *fila = list_find(tlb.lista, (void*) es_el_buscado);
    if (fila == NULL) {

        log_info(logger, "PID: %i - TLB MISS - Pagina: %i", pid, pagina);
        log_destroy(logger);

        return -1;
    }

    log_info(logger, "PID: %i - TLB HIT - Pagina: %i", pid, pagina);
    log_destroy(logger);

    fila->tiempo_referencia = temporal_gettime(tlb.tiempo_inicio);
    return fila->frame;
}

void tlb_agregar_fila(int pid, int pagina, int frame) {
    t_fila_tlb *fila = malloc(sizeof(t_fila_tlb));
    fila->pid = pid;
    fila->pagina = pagina;
    fila->frame = frame;
    fila->tiempo_carga = temporal_gettime(tlb.tiempo_inicio);
    fila->tiempo_referencia = fila->tiempo_carga;

    if (tlb.cant_filas < config_cpu.cantidad_entradas_tlb) {
        list_add(tlb.lista, fila);
        tlb.cant_filas++;
    }
    else {
        // Debo reemplazar
        if (string_equals_ignore_case(config_cpu.algoritmo_tlb, "LRU")) {
            tlb_reemplazar_fila_LRU(fila);
        }
        else {
            tlb_reemplazar_fila_FIFO(fila);
        }
    }
}

void tlb_reemplazar_fila_FIFO(t_fila_tlb *fila_nueva) {
    t_fila_tlb *carga_mas_antigua(t_fila_tlb* fila1, t_fila_tlb* fila2) {
        if (fila2->tiempo_carga < fila1->tiempo_carga) {
            return fila2;
        }
        return fila1;
    }

    t_fila_tlb *victima = list_get_minimum(tlb.lista, (void*) carga_mas_antigua);

    list_remove_element(tlb.lista, (void*) victima);
    free(victima);

    list_add(tlb.lista, fila_nueva);
}

void tlb_reemplazar_fila_LRU(t_fila_tlb *fila_nueva) {
    t_fila_tlb *referencia_mas_antigua(t_fila_tlb* fila1, t_fila_tlb* fila2) {
        if (fila2->tiempo_referencia < fila1->tiempo_referencia) {
            return fila2;
        }
        return fila1;
    }

    t_fila_tlb *victima = list_get_minimum(tlb.lista, (void*) referencia_mas_antigua);

    list_remove_element(tlb.lista, (void*) victima);
    free(victima);

    list_add(tlb.lista, fila_nueva);
}

/* void eliminar_tlb() {
    // eliminar lista
    //// list_destroy(tlb.lista);
    temporal_destroy(tlb.tiempo_inicio);
    free(tlb);
} */