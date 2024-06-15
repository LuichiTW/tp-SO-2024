#include "main.h"

/*
? CHECK POINT 2
!Se conecta a Kernel✅ y recibe un PCB❎.
!Es capaz de conectarse a la memoria✅ y solicitar las instrucciones❎.
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
!MOV_IN❎
!MOV_OUT❎
!RESIZE❎
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

void recibir_pcb(int socket_cliente_kernel_dispatch, int socket_cliente_kernel_interrupt);
sem_t s_interrupcion;
t_pcb *pcb;

int main() {
    loggerPrincipal = log_create("cpu.log", "cpu", true, LOG_LEVEL_INFO);
	log_info(loggerPrincipal, "Iniciando CPU...");
    t_config *config = config_create("./cpu.config");
    if (config == NULL)
    {
        log_error(loggerPrincipal, "No se leyo el archivo de configuracion");
        exit(EXIT_FAILURE);
    }


//! se omite la conexiones entre modulos para hacer pruebas independientemente en el modulo
  
    //* CLIENTE------------------------------------------>
    char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    int conexion_memoria = crear_conexion(ip_memoria,puerto_memoria,"Hola soy el CPU");
    //*SERVIDOR------------------------------------------>
        //*DISPATCH
	char *puerto_dispatch = config_get_string_value(config, "PUERTO_ESCUCHA_DISPATCH");
    int socket_servidor_cpu_dispatch = iniciar_servidor(puerto_dispatch);
	log_info(loggerPrincipal, "Listo para recibir al Kernel en dispatch");
    int socket_cliente_kernel_dispatch = esperar_cliente(socket_servidor_cpu_dispatch, loggerPrincipal);
        //*INTERRUPT
    char *puerto_interrupt = config_get_string_value(config, "PUERTO_ESCUCHA_INTERRUPT");
    int socket_servidor_cpu_interrupt = iniciar_servidor(puerto_dispatch);
	log_info(loggerPrincipal, "Listo para recibir al Kernel en interrupt");
    int socket_cliente_kernel_interrupt = esperar_cliente(socket_servidor_cpu_interrupt, loggerPrincipal);


    sem_init(&interrupcion, 0, 0);

    recibir_pcb(socket_cliente_kernel_dispatch, socket_cliente_kernel_interrupt);

//! PRUEBA DE CICLO DE INSTRUCCION MANUAL
 /*
    while (1) {
        char *test = funFetch(1);
        if (strcmp(test,"0") == 0) {
            break;
        }
        struct s_instruccion *instruccionDecodificada = funDecode(test);
        funExecute(instruccionDecodificada);
        eliminar_Lista_Instruccion(instruccionDecodificada);
        funCheckInterrupt(pcb, socket_cliente_kernel_interrupt);
    }

 */
    return 0;
}


void recibir_pcb(int socket_cliente_kernel_dispatch, int socket_cliente_kernel_interrupt){
    //recibir PCB
    t_pcb *pcb = malloc(sizeof(t_pcb));
    int bytesRecibidos = recv(socket_cliente_kernel_dispatch, pcb, sizeof(t_pcb), 0);
    if (bytesRecibidos <= 0) {
        log_error(loggerPrincipal, "No se pudo recibir el PCB");
        exit(EXIT_FAILURE);
    }
    log_info(loggerPrincipal, "Se recibio el PCB correctamente");
    }

   
