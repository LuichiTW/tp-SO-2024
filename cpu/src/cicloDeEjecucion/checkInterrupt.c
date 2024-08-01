#include "checkInterrupt.h"

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
            // Guardar el motivo de la interrupción en algún lado
            //char *motivoInterrupt = recibir_msg(sockets_cpu.socket_kernel_interrupt);
            hay_interrupcion = true;
        }
    }
    
}


void funCheckInterrupt(){
    if (hay_interrupcion) {
        if (proceso_ejecutando) {
            devolver_contexto_ejecucion("INTERRUPT"); // TODO agregar motivo de interrupción
        }
        hay_interrupcion = false;
    }
}

void devolver_contexto_ejecucion(char *motivo_desalojo) {
    actualizar_pcb();
    sem_wait(&mutex_dispatch);

    t_paquete *paquete = empaquetar_pcb(&pcb);
    agregar_a_paquete(paquete, motivo_desalojo, string_length(motivo_desalojo)+1);
    enviar_peticion(paquete, sockets_cpu.socket_kernel_dispatch, KER_CDE);
    eliminar_paquete(paquete);
    
    sem_post(&mutex_dispatch);
    proceso_ejecutando = false;
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

