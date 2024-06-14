#include "main.h"

/*
? CHECK POINT 2
Se conecta a Kernel✅ y recibe un PCB❎.
Es capaz de conectarse a la memoria✅ y solicitar las instrucciones❎.
Es capaz de ejecutar un ciclo básico de instrucción❎.
    fetch❎
        ir a memoria a buscar proxima instruccion (con el PC) -> se almacena en un char
    deocde✅
        traducir la instruccion recibida de la memoria
            1ro separa el char recibido de memoria en un array segun la cantidd de elementos que tenga
            4to armar un struc con toda la informacion de la instruccion a ejecutar
        traducir las direcciones logicas a fisicas si es requerido
    execute✅/❎
        teniendo el struc con toda la informacion de instruccion a ejecutar se busca la instruccion en un switch
    check interruption❎
        cheuqemeamos un booleano que indicara si se recibio del kernel una instruccion, este booleano sera manimulado mediante un semaforo el cual va a estar comunicandose entre dos hilos del proceso para evisar que haya deadlock
        en caso de que haya una interrupcion, se enviara el PCB al kernel para que lo guarde y posteriormente se cargara la nueva pcb para ejecutar la interrucion
        al termino de ejecutar la interrupcion se revisara si hay otra interrucion, en caso de que haya se repetira lo anterior
        en caso de que no haya se le eniara al jernel el pcb de la interrupcion y se solicitara la proxima pcb a ejecutar
    sumar +1 al PC
Es capaz de resolver las operaciones: SET✅, SUM✅, SUB✅, JNZ✅ e IO_GEN_SLEEP❎.

? CHECK POINT 3
Es capaz de resolver las operaciones: MOV_IN❎, MOV_OUT❎, RESIZE❎, COPY_STRING❎, IO_STDIN_READ❎, IO_STDOUT_WRITE❎.


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
/*     
    char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    int conexion_memoria = crear_conexion(ip_memoria,puerto_memoria,"Hola soy el CPU");

	char *puerto_dispatch = config_get_string_value(config, "PUERTO_ESCUCHA_DISPATCH");
    int socket_servidor_cpu = iniciar_servidor(puerto_dispatch);
	log_info(loggerPrincipal, "Listo para recibir al Kernel");
    int socket_cliente_kernel = esperar_cliente(socket_servidor_cpu, loggerPrincipal);
 */
    struct s_instruccion *instruccionDecodificada = funDecode("JNZ AX 100");
    funExecute(instruccionDecodificada);

    char *test=readline("> ");
    return 0;
}
