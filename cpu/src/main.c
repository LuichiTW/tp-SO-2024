#include "main.h"

/*
? CHECK POINT 2
!Se conecta a Kernel✅ y recibe un PCB✅.
!Es capaz de conectarse a la memoria✅ y solicitar las instrucciones✅.
!Es capaz de ejecutar un ciclo básico de instrucción❎✅✅❎.
!    fetch❎
        ir a memoria a buscar proxima instruccion (con el PC) -> se almacena en un char
*       sumar +1 al PC
*   decode✅
*    execute✅
!    check interruption❎
        chequeamos un booleano que indicara si se recibio del kernel una instruccion, este booleano sera manipulado mediante un semaforo el cual va a estar comunicandose entre los hilos del proceso para evitar que haya deadlock
        en caso de que haya una interrupcion, se enviara el PCB al kernel para que lo guarde y posteriormente se cargara la nueva pcb para ejecutar la interrucion
        al termino de ejecutar la interrupcion se revisara si hay otra interrucion, en caso de que haya se repetira lo anterior
        en caso de que no haya se le eniara al jernel el pcb de la interrupcion y se solicitara la proxima pcb a ejecutar
!Es capaz de resolver las operaciones: 
    *SET✅ 
    *SUM✅ 
    *SUB✅ 
    *JNZ✅
    !IO_GEN_SLEEP❎.

? CHECK POINT 3
!Es capaz de resolver las operaciones:
!MOV_IN✅
!MOV_OUT✅
!RESIZE✅
!COPY_STRING❎
!IO_STDIN_READ❎
!IO_STDOUT_WRITE❎


? <-------------FINAL---------------->

LOGS MINIMOS Y OBLIGATORIOS
Fetch Instrucción❎: PID: <PID> - FETCH - Program Counter: <PROGRAM_COUNTER>.
Instrucción Ejecutada❎: PID: <PID> - Ejecutando: <INSTRUCCION> - <PARAMETROS>.
TLB Hit❎: PID: <PID> - TLB HIT - Pagina: <NUMERO_PAGINA>
TLB Miss❎: PID: <PID> - TLB MISS - Pagina: <NUMERO_PAGINA>
Obtener Marco❎: PID: <PID> - OBTENER MARCO - Página: <NUMERO_PAGINA> - Marco: <NUMERO_MARCO>
Lectura/Escritura Memoria❎: PID: <PID> - Acción: <LEER / ESCRIBIR> - Dirección Física: <DIRECCION_FISICA> - Valor: <VALOR LEIDO / ESCRITO>
*/

//variable global de los registros del CPU, se usa el Keyword extern para poder hacer uso de estos registros.
registros_cpu regcpu;
//log global del CPU
t_log *loggerPrincipal;


sem_t s_interrupcion;

int main() {
    t_log *logger = log_create("cpu.log", "cpu", true, LOG_LEVEL_INFO);
    t_log *alt_logger = log_create("cpu_extra.log", "cpu extra", true, LOG_LEVEL_INFO);

	log_info(alt_logger, "Iniciando CPU...");
    cargar_config();
    cargar_sockets();

    inicializar_tlb();
    crear_thread_interrupt();

    // ? Ignorando el código que conecta con kernel para debug
    ////sockets_cpu.socket_cliente_kernel_interrupt = esperar_cliente(sockets_cpu.socket_servidor_cpu_interrupt, loggerPrincipal);

    ////sem_init(&s_interrupcion, 0, 0);

    ////pcb = recibir_pcb(sockets_cpu.socket_servidor_cpu_dispatch, sockets_cpu.socket_servidor_cpu_interrupt);

    // Ciclo de instrucción
    while (1) {
        // Si no tiene un proceso ejecutando, espera a recibir uno de Kernel.
        if (!proceso_ejecutando) {
            recibir_pcb();
        }

        // Fetch
        char *instruccionStr = funFetch(sockets_cpu.socket_memoria);

        // Decode
        struct s_instruccion *instruccionDecodificada = funDecode(instruccionStr);

        // Execute
        funExecute(instruccionDecodificada);

        // Log de instrucción ejecutada
        char **instYParametros = string_n_split(instruccionStr, 2, " ");
        if (string_array_size(instYParametros) < 2) {
            log_info(logger, "PID: %i - Ejecutando: %s", pcb.pid, instYParametros[0]);
        }
        else {
            log_info(logger, "PID: %i - Ejecutando: %s - %s", pcb.pid, instYParametros[0], instYParametros[1]);
        }

        // Check interrupt
        funCheckInterrupt();

        // Limpieza de memoria
        free(instruccionStr);
        string_array_destroy(instYParametros);
        eliminar_Lista_Instruccion(instruccionDecodificada);
    }
    log_destroy(logger);
    log_destroy(alt_logger);

    return 0;
}

   
