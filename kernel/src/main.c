#include "main.h"

typedef struct
    {
        int PID;     // Identificador del proceso
        int PC;      // Contador de Programa
        int Quantum; // Unidad de tiempo utilizada para el algoritmo VRR
        //necesito el tipo de dato de los registros que vienen del cpu
    }pcb;

typedef struct 
{
    pcb PCBS;
    struct NodoColaPCBS*sig;
}NodoColaPCBS;

int main() {
    //Iniciar logger del kernel y su config
    t_log *logger = log_create("kernel.log", "kernel", true, LOG_LEVEL_INFO);
    log_info(logger, "Iniciando kernel...");
    t_config *config = config_create("../kernel.config");
    if (config == NULL) {
        log_error(logger, "No se leyo el archivo de configuracion");
        exit(EXIT_FAILURE);
    }

    //iniciar el cliente del kernel

    //memoria
    char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    int conexion_memoria = crear_conexion(ip_memoria, puerto_memoria, "Hola soy el kernel");
    
    //cpu
    char *puerto_cpu = config_get_string_value(config, "PUERTO_CPU_DISPATCH");
    char *ip_cpu = config_get_string_value(config, "IP_CPU");
    int conexion_cpu = crear_conexion(ip_cpu, puerto_cpu, "Hola soy el kernel");

    //iniciar el servidor del kernel

    //int conexion_io;
	char *puerto = config_get_string_value(config, "PUERTO_ESCUCHA");
    int socket_servidor_kernel = iniciar_servidor(puerto);
	log_info(logger, "Listo para recibir al IO");
    int socket_cliente_io = esperar_cliente(socket_servidor_kernel, logger);
    
    //char* algo = readline("> "); // Deshabilitado para probar la consola
    //se tendria que liberar el especio de memoria usado por los elementos

    //CONSOLA INTERACTIVA
    iniciar_consola_interactiva(logger,conexion_cpu);
    return 0;
}

void iniciar_consola_interactiva(t_log*logger,int conexion_cpu)
{
    printf("Bienvenido a la Consola Interactiva de Kernel. Ingrese una funcion:\n"
    " -   EJECUTAR_SCRIPT\n"
    " -   INICIAR_PROCESO\n"
    " -   FINALIZAR_PROCESO\n"
    " -   DETENER_PLANIFICACION\n"
    " -   INICIAR_PLANIFICACION\n"
    " -   MULTIPROGRAMACION\n"
    " -   PROCESO_ESTADO\n"
    "\n");
    char* leido;
    leido = readline("> ");
    bool validacion_leido;

    while (strcmp(leido,"\0") != 0)
    {
        validacion_leido= validacion_de_instruccion_de_consola(leido, logger);
        if (!validacion_leido)
        {
            log_error (logger,"Comando de CONSOLA no reconocido, por favor ingrese un comando de nuevo");
            free(leido);
            leido = readline(">");
            continue; //Saltar y continuar con el resto de la iteracion
        }
        atender_instruccion_valida(leido, logger, conexion_cpu);
        free(leido);
        leido = readline("> ");
    }
    free(leido);
}

void atender_instruccion_valida(char*leido, t_log*logger, int conexion_cpu)
{
    //VERIFICACION DE QUE COMANDO SE ESTA LLAMANDO
    char* comando_consola = strtok(leido, " ");
    int opcion_valida=0;
    if (strcmp(comando_consola,"EJECUTAR_SCRIPT")==0)
        opcion_valida=1;
    else if (strcmp(comando_consola,"INICIAR_PROCESO")==0)
        opcion_valida=2;
    else if (strcmp(comando_consola,"FINALIZAR_PROCESO")==0)
        opcion_valida=3;
    else if (strcmp(comando_consola,"DETENER_PLANIFICACION")==0)
        opcion_valida=4;
    else if (strcmp(comando_consola,"INICIAR_PLANIFICACION")==0)
        opcion_valida=5;
    else if (strcmp(comando_consola,"MULTIPROGRAMACION")==0)
        opcion_valida=6;
    else if (strcmp(comando_consola,"PROCESO_ESTADO")==0)
        opcion_valida=7;
    else
        log_error(logger,"Error durante la validacion de la instruccion en la consola.");
    
    switch (opcion_valida)
    {
    case 1: //EJECUTAR_SCRIPT
        pid_pcb++;
        char*pid_pcb_char;
        sprintf(pid_pcb_char,"%d",pid_pcb);
        enviar_mensaje(pid_pcb_char,conexion_cpu);
        break;
    case 2: //INICIAR_PROCESO
        break;
    case 3: //FINALIZAR_PROCESO
        break;
    case 4: //DETENER_PLANIFICACION
        break;
    case 5: //INICIAR_PLANIFICACION
        break;
    case 6: //MULTIPROGRAMACION
        break;
    case 7: //PROCESO_ESTADO
        break;
    default:
        log_error(logger,"Error durante la validacion de la instruccion en la consola.");
        break;
    }
}

bool validacion_de_instruccion_de_consola(char* leido, t_log*logger)
{
    char* comando_consola = strtok(leido, " ");
    bool opcion_valida=false;
    if (strcmp(comando_consola,"EJECUTAR_SCRIPT")==0)
        opcion_valida=true;
    else if (strcmp(comando_consola,"INICIAR_PROCESO")==0)
        opcion_valida=true;
    else if (strcmp(comando_consola,"FINALIZAR_PROCESO")==0)
        opcion_valida=true;
    else if (strcmp(comando_consola,"DETENER_PLANIFICACION")==0)
        opcion_valida=true;
    else if (strcmp(comando_consola,"INICIAR_PLANIFICACION")==0)
        opcion_valida=true;
    else if (strcmp(comando_consola,"MULTIPROGRAMACION")==0)
        opcion_valida=true;
    else if (strcmp(comando_consola,"PROCESO_ESTADO")==0)
        opcion_valida=true;
    else
    {
        log_error(logger,"Comando no reconocido.");
        opcion_valida=false;
    }
    return opcion_valida;
}