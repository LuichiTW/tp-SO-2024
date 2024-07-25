#include "fetch.h"

char *funFetch(int socketMemoria){
    // ! Valores hardcodeados

    uint32_t numero = regcpu.PC;
    int pid = 1;

    // ! --------------------
    
    t_paquete* paquete = crear_paquete();
    agregar_a_paquete(paquete, &pid, sizeof(uint32_t));
    agregar_a_paquete(paquete, &numero, sizeof(uint32_t));
    enviar_peticion(paquete, socketMemoria, MEM_ENVIAR_INSTRUCCION);
    free(paquete);

    //t_log *alt_logger = log_create("cpu_extra.log", "cpu", true, LOG_LEVEL_INFO);
    t_log *logger = log_create("cpu.log", "cpu", true, LOG_LEVEL_INFO);
    /* int op =  */recibir_operacion(socketMemoria);
    //log_info(alt_logger, "op: %i", op);

    char *instruccion = recibir_msg(socketMemoria);
    log_info(logger, "PID: %i - FETCH - Program Counter: %i", pid, numero);

    log_destroy(logger);
    regcpu.PC++;
    return instruccion;
}

