#include "interrupciones.h"

void interrumpir(char *motivo) {
    t_paquete *paquete = crear_paquete();
    agregar_a_paquete(paquete, motivo, string_length(motivo)+1);
    enviar_peticion(paquete, sockets.cpu_interrupt, CPU_INTERRUPT);
    free(paquete);
}