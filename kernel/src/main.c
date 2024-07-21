#include <main.h>

int main() {

    //cola NEW
    Cola*colaNEW=(Cola*)malloc(sizeof(Cola));
    colaNEW->primero=colaNEW->ultimo=NULL;;
    //cola READY
    Cola*colaREADY=(Cola*)malloc(sizeof(Cola));
    colaREADY->primero=colaREADY->ultimo=NULL;
    //cola FIFO
    Cola*colaFIFO=(Cola*)malloc(sizeof(Cola));
    colaFIFO->primero=colaFIFO->ultimo=NULL;
    //cola RR
    Cola*colaRR=(Cola*)malloc(sizeof(Cola));
    colaRR->primero=colaRR->ultimo=NULL;
    //cola VRR
    Cola*colaVRR=(Cola*)malloc(sizeof(Cola));
    colaVRR->primero=colaVRR->ultimo=NULL;

    // INICIAR LOGGER DEL KERNEL Y SU CONFIG
    t_log *logger = log_create("kernel.log", "kernel", true, LOG_LEVEL_INFO);
    log_info(logger, "Iniciando kernel...");
    t_config *config = config_create("../kernel.config");
    if (config == NULL) {
        log_error(logger, "No se leyo el archivo de configuracion");
        exit(EXIT_FAILURE);
    }

    // CONECTAR COMO CLIENTE

    // MEMORIA
    char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    int conexion_memoria = crear_conexion(ip_memoria, puerto_memoria, "Hola soy el kernel");
    
    // CPU
    char *puerto_cpu = config_get_string_value(config, "PUERTO_CPU_DISPATCH");
    char *ip_cpu = config_get_string_value(config, "IP_CPU");
    int conexion_cpu = crear_conexion(ip_cpu, puerto_cpu, "Hola soy el kernel");

    // INICIAR SERVIDOR PARA 

	char *puerto = config_get_string_value(config, "PUERTO_ESCUCHA");
    int socket_servidor_kernel = iniciar_servidor(puerto);
	log_info(logger, "Listo para recibir al IO");
    int socket_cliente_io = esperar_cliente(socket_servidor_kernel, logger);

    //se tendria que liberar el especio de memoria usado por los elementos

    //CONSOLA INTERACTIVA
    iniciar_consola_interactiva(logger,conexion_cpu,conexion_memoria,colaNEW,colaREADY);
    return 0;
}

void iniciar_consola_interactiva(t_log*logger,int conexion_cpu,int conexion_memoria,Cola*colaNEW,Cola*colaREADY,Cola*colaFIFO,Cola*colaRR,Cola*colaVRR)
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
    char leido[40];
    printf(">");
    gets(leido);
    bool validacion_leido;
    while (strcmp(leido,"\0") != 0)
    {
        validacion_leido= validacion_de_instruccion_de_consola(leido, logger);
        if (!validacion_leido)
        {
            log_error (logger,"Comando de CONSOLA no reconocido, por favor ingrese un comando de nuevo");
            free(leido);
            gets(leido);
            continue;
        }
        atender_instruccion_valida(leido, logger, conexion_cpu, conexion_memoria,colaNEW,colaREADY);
        gets(leido);
    }
    free(leido);
}

void atender_instruccion_valida(char*leido, t_log*logger, int conexion_cpu,int conexion_memoria,Cola*colaNEW,Cola*colaREADY,Cola*colaFIFO,Cola*colaRR,Cola*colaVRR)
{
    int PIDs=0;
    char* comando_consola = strtok(leido, " ");
    char* path = strtok (NULL," ");
    printf("%s",path);

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
        int cont=0;
        char linea[30];

        t_config *config = config_create("../kernel.config");
        int grado_multiprogramacion = config_get_int_value(config,"GRADO_MULTIPROGRAMACION");

        FILE*archivo_de_instrucciones;
        archivo_de_instrucciones=fopen(path,"r");
        fgets(linea,30,archivo_de_instrucciones);
        while (!feof(archivo_de_instrucciones))
        {
            char* instruccion = strtok(linea, " ");
            if (strcmp(instruccion,"INICIAR_PROCESO"))
            {
                &PIDs++;
                log_info(logger,"Se crea el proceso <%d> en NEW",PIDs);
                char* path = strtok(NULL, " ");
                iniciar_proceso(path,PIDs,conexion_cpu,conexion_memoria,colaNEW);
                enviar_mensaje("Crea un proceso cuyas operaciones se encuentran en",conexion_memoria);
                enviar_mensaje(path,conexion_memoria);
            }
            fgets(linea,30,archivo_de_instrucciones);
        }
        fclose(archivo_de_instrucciones);
        if (PIDs<grado_multiprogramacion)
        {
            
            encolarColaREADY(colaNEW,colaREADY);
        }
        
        break;
    case 2: //INICIAR_PROCESO
        PIDs++;
        iniciar_proceso(path,PIDs,conexion_cpu,conexion_memoria,colaNEW);
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

void iniciar_proceso(char*path,int PID,int conexion_cpu,int conexion_memoria,Cola*colaNEW){
    pcb proceso;

    t_config *config = config_create("../kernel.config");
    int quantum = config_get_int_value(config,"QUANTUM");

    proceso.PID=PID;
    proceso.PC=(PID++);
    proceso.Quantum=quantum;
    encolarColaNEW(proceso,colaNEW);
    enviar_mensaje("Enviando nuevo PCB a CPU...",conexion_cpu);
    enviar_mensaje(PID,conexion_cpu);
    enviar_pcb(proceso,conexion_cpu);
}

void enviar_pcb(pcb proceso, int conexion_cpu){
}

void encolarColaNEW(pcb ProcesoNuevo,Cola*colaNEW)
{   
    NodoColaPCBS*nuevo;
    nuevo=malloc(sizeof(NodoColaPCBS));

    nuevo->PCBS.PID=ProcesoNuevo.PID;
    nuevo->PCBS.PC=ProcesoNuevo.PC;
    nuevo->PCBS.Quantum=ProcesoNuevo.Quantum;
    nuevo->sig=NULL;
    if (colaNEW->ultimo!=NULL)
    {
        colaNEW->ultimo->sig=nuevo;
    }
    else
    {
        colaNEW->primero=nuevo;
    }
    colaNEW->ultimo=nuevo;
}

void encolarColaREADY(Cola*colaNEW,Cola*colaREADY,Cola*colaFIFO,Cola*colaRR,Cola*colaVRR)
{   

    NodoColaPCBS*nuevo;
    nuevo=malloc(sizeof(NodoColaPCBS));
    NodoColaPCBS*puntero;
    puntero=colaNEW->primero;

    while (puntero!=NULL)
    {
        nuevo->PCBS.PID=puntero->PCBS.PID;
        nuevo->PCBS.PC=puntero->PCBS.PC;
        nuevo->PCBS.Quantum=puntero->PCBS.Quantum;
        nuevo->sig=NULL;

        if (colaREADY->ultimo!=NULL)
        {
            colaREADY->ultimo->sig=nuevo;
        }
        else
        {
            colaREADY->primero=nuevo;
        }
        colaREADY->ultimo=nuevo;
        puntero=puntero->sig;
    }

    t_config *config = config_create("../kernel.config");
    char* algoritmo = config_get_int_value(config,"ALGORITMO_PLANIFICACION");
    //---
    if (strcmp(algoritmo,"FIFO")==0)
        encolarColaFIFO(colaREADY,colaFIFO);
    else if (strcmp(algoritmo,"RR")==0)
        encolarColaFIFO(colaREADY,colaRR);
    else if (strcmp(algoritmo,"VRR")==0)
        encolarColaFIFO(colaREADY,colaVRR);
    else
    {
        printf("El algoritmo del .config no es valido");
    }
    
}

void encolarColaFIFO(Cola*colaREADY,Cola*colaFIFO)

{   
    //ENCOLAR PROCESOS

    NodoColaPCBS*puntero;
    puntero=colaREADY->primero;

    while (puntero!=NULL)
    {
        NodoColaPCBS*nuevo;
        nuevo=malloc(sizeof(NodoColaPCBS));
        nuevo->PCBS.PID=puntero->PCBS.PID;
        nuevo->PCBS.PC=puntero->PCBS.PC;
        nuevo->PCBS.Quantum=puntero->PCBS.Quantum;
        nuevo->sig=NULL;

        if (colaFIFO->ultimo!=NULL)
        {
            colaFIFO->ultimo->sig=nuevo;
        }
        else
        {
            colaFIFO->primero=nuevo;
        }
        colaFIFO->ultimo=nuevo;
        puntero=puntero->sig;
    }
}

void encolarColaRR(Cola*colaREADY,Cola*colaRR)
{   
    //ENCOLAR PROCESOS

    NodoColaPCBS*puntero;
    puntero=colaREADY->primero;

    while (puntero!=NULL)
    {
        NodoColaPCBS*nuevo;
        nuevo=malloc(sizeof(NodoColaPCBS));
        nuevo->PCBS.PID=puntero->PCBS.PID;
        nuevo->PCBS.PC=puntero->PCBS.PC;
        nuevo->PCBS.Quantum=puntero->PCBS.Quantum;
        nuevo->sig=NULL;

        if (colaRR->ultimo!=NULL)
        {
            colaRR->ultimo->sig=nuevo;
        }
        else
        {
            colaRR->primero=nuevo;
        }
        colaRR->ultimo=nuevo;
        puntero=puntero->sig;
    }

    //ENVIAR CONTEXTO DE EJECUCION

    t_temporal* cronometro = temporal_create();
    int quantum_total;
    //ESPERO MENSAJE DE CPU

    temporal_stop(cronometro);
    int64_t tiempo = temporal_gettime(cronometro);

    if (tiempo>quantum_total)
    {
        /* code */
    }
}

void encolarColaVRR(Cola*colaREADY,Cola*colaVRR)
{   
    NodoColaPCBS*puntero;
    puntero=colaNEW->primero;

    while (puntero!=NULL)
    {
        NodoColaPCBS*nuevo;
        nuevo=malloc(sizeof(NodoColaPCBS));
        nuevo->PCBS.PID=puntero->PCBS.PID;
        nuevo->PCBS.PC=puntero->PCBS.PC;
        nuevo->PCBS.Quantum=puntero->PCBS.Quantum;
        nuevo->sig=NULL;

        if (colaVRR->ultimo!=NULL)
        {
            colaVRR->ultimo->sig=nuevo;
        }
        else
        {
            colaVRR->primero=nuevo;
        }
        colaVRR->ultimo=nuevo;
        puntero=puntero->sig;
        printf("");
        printf("VRR");
    }
}