#include "checkInterrupt.h"

//t_pcb *pcb;
bool hay_interrupcion;

void crear_thread_interrupt() {
    pthread_t thread_interr;
    pthread_create(&thread_interr, NULL, (void*)recibir_interrupciones, NULL);
    pthread_detach(thread_interr);
}


void recibir_interrupciones() {
    while (true) {    
        int op = recibir_operacion(sockets_cpu.socket_kernel_interrupt);
        if (op == CPU_INTERRUPT) {
            hay_interrupcion = true;
        }
    }
    
}


void funCheckInterrupt(){
    if (hay_interrupcion) {
        devolver_contexto_ejecucion();
        hay_interrupcion = false;
    }
}

void devolver_contexto_ejecucion(enum motivo_desalojo motivo_desalojo) {
    actualizar_pcb();
    // Enviar a kernel el CDE y Motivo desalojo
    // ? Esperar otro CDE de kernel?
}


/* void checkInterrupt(t_parametroCheckInterrupt *parametros){
    while (1)
    {   
        int op = recibir_operacion(parametros->socket_cliente_kernel_interrupt);
        if(op != -1){
        //enviar pcb
        int bytesEnviados = send(parametros->socket_cliente_kernel_interrupt, parametros
        ->pcb, sizeof(t_pcb), 0);
        if (bytesEnviados <= 0) {
            log_error(loggerPrincipal, "No se pudo enviar el PCB");
            exit(EXIT_FAILURE);
        }
        log_info(loggerPrincipal, "Se envio el PCB correctamente");
    }
    else{
        log_info(loggerPrincipal,"No hay interrupciones");
    }
    }
    
} */

