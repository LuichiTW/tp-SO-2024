#include "checkInterrupt.h"

t_pcb *pcb;

void funCheckInterrupt(t_pcb *pcb, int socket_cliente_kernel_interrupt){
    
    sem_wait(&s_interrupcion);
    pthread_t kernel_t;
    t_parametroCheckInterrupt *parametros = malloc(sizeof(t_parametroCheckInterrupt));
    parametros->pcb = pcb;
    parametros->socket_cliente_kernel_interrupt = socket_cliente_kernel_interrupt;
    pthread_create(&kernel_t, NULL, (void*)checkInterrupt,(void*)&parametros);
    pthread_join(kernel_t, NULL);
    pthread_detach(kernel_t);
    sem_post(&s_interrupcion);
}

void checkInterrupt(t_parametroCheckInterrupt *parametros){
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
    
}

