#include <main.h>

sem_t sem;
// TODO Iniciar escucha io
// TODO Usar semáforo para canal dispatch
int main() 
{
    cargar_config();
    levantar_conexiones();
    inicializar_colas();

    pthread_t thread_consola;
    pthread_create(&thread_consola, NULL, (void*)iniciar_consola, NULL);
    pthread_detach(thread_consola);

    /* pthread_t thread_planificador;
    pthread_create(&thread_planificador, NULL, (void*)planificar, NULL);
    pthread_detach(thread_planificador); */

    sem_init(&sem, false, 0);
    sem_wait(&sem);

    return 0;
}
/*
void iniciar_consola_interactiva(t_log*logger,int conexion_cpu_dispatch,int conexion_cpu_interrupt,int conexion_memoria,Cola*colaNEW,Cola*colaREADY,Cola*colaFIFO,Cola*colaRR,Cola*colaVRR,Cola*colaEXIT)
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
        atender_instruccion_valida(leido,logger,conexion_cpu_dispatch,conexion_cpu_interrupt,conexion_memoria,colaNEW,colaREADY,colaFIFO,colaRR,colaVRR,colaEXIT);
        gets(leido);
    }
    free(leido);
}

void atender_instruccion_valida(char*leido, t_log*logger, int conexion_cpu_dispatch,int conexion_cpu_interrupt,int conexion_memoria,Cola*colaNEW,Cola*colaREADY,Cola*colaFIFO,Cola*colaRR,Cola*colaVRR,Cola*colaEXIT)
{
    int PIDs=0;
    char* comando_consola = strtok(leido, " ");
    char* path = strtok (NULL," ");

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
                    PIDs++;
                    log_info(logger,"Se crea el proceso <%d> en NEW",PIDs);
                    char* path = strtok(NULL, " ");
                    iniciar_proceso(path,PIDs,conexion_cpu_dispatch,conexion_memoria,colaNEW);
                    enviar_mensaje("Crea un proceso cuyas operaciones se encuentran en",conexion_memoria);
                    enviar_mensaje(path,conexion_memoria);
                }
                fgets(linea,30,archivo_de_instrucciones);
            }
            fclose(archivo_de_instrucciones);
            if (PIDs<grado_multiprogramacion)
            {
                
                encolarColaREADY(colaNEW,colaREADY,colaFIFO,colaRR,colaVRR);
            }
            break;
        case 2: //INICIAR_PROCESO
            PIDs++;
            iniciar_proceso(path,PIDs,conexion_cpu_dispatch,conexion_memoria,colaNEW);
            break;
        case 3: //FINALIZAR_PROCESO
            int pid = (int)strtol(path,NULL,10);
            enviar_mensaje("FInalizar EL PROCESO CON PID ENVIADO A CONTINUACION",conexion_cpu_interrupt);
            enviar_entero(pid,conexion_cpu_interrupt);
            log_info(logger,"FINALIZA EL PROCESO <%d> - Motivo: <SUCCESS / INVALID_RESOURCE / INVALID_WRITE>",pid);
            break;
        case 4: //DETENER_PLANIFICACION
            break;
        case 5: //INICIAR_PLANIFICACION
            break;
        case 6: //MULTIPROGRAMACION
            char* linea_leida;
            FILE*archivo_config;
            archivo_config=fopen("kernel.dat","rb+");
            if (!archivo_config)
            {
                printf("No se pudo abrir el archivo");
            }
            fgets(linea_leida,50,archivo_config);
            while (!feof(archivo_config))
            {
                if ((strcmpi(linea_leida,"GRADO_MULTIPROGRAMACION=10"))==0)
                {
                    readline(">");
                    fseek(archivo_config,(-2)*sizeof(archivo_config),SEEK_CUR);
                }
                fgets(linea,50,archivo_config);
            }
            fclose(archivo_config);
            break;
        case 7: //PROCESO_ESTADO
            listarProcesos(colaNEW,"NEW",logger);

            listarProcesos(colaREADY,"READY",logger);
            char* algoritmo = config_get_string_value(config,"ALGORITMO_PLANIFICACION");

            if (strcmp(algoritmo,"FIFO")==0)
                listarProcesos(colaFIFO,"EXEC",logger);
            else if (strcmp(algoritmo,"RR")==0)
                listarProcesos(colaRR,"EXEC",logger);
            else if (strcmp(algoritmo,"VRR")==0)
                listarProcesos(colaVRR,"EXEC",logger);

            listarProcesos(colaEXIT,"EXEC",logger);
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

void iniciar_proceso(char*path,int PID,int conexion_cpu,int conexion_memoria,Cola*colaNEW)
{
    pcb proceso;

    t_config *config = config_create("../kernel.config");
    int quantum = config_get_int_value(config,"QUANTUM");

    proceso.PID=PID;
    proceso.PC=(PID++);
    proceso.Quantum=quantum;
    encolarColaNEW(proceso,colaNEW);
    enviar_mensaje("Enviando nuevo PCB a CPU...",conexion_cpu);
    enviar_pcb(proceso,conexion_cpu);
}

void enviar_pcb(pcb proceso, int conexion_cpu)
{
    enviar_entero(proceso.PID,conexion_cpu);
    enviar_entero(proceso.PC,conexion_cpu);
    enviar_entero(proceso.Quantum,conexion_cpu);
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
        colaNEW->ultimo=nuevo;
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
            colaREADY->ultimo=nuevo;
        }
        else
        {
            colaREADY->primero=nuevo;
        }
        colaREADY->ultimo=nuevo;
        puntero=puntero->sig;
        colaNEW->primero=puntero;
    }

    t_config *config = config_create("../kernel.config");
    char* algoritmo = config_get_string_value(config,"ALGORITMO_PLANIFICACION");

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
            colaFIFO->ultimo=nuevo;
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
            colaRR->ultimo=nuevo;
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
    int quantum_total = puntero->PCBS.Quantum;

    //ESPERO MENSAJE DE CPU Y PARO EL CRONOMETRO

    temporal_stop(cronometro);
    int64_t tiempo = temporal_gettime(cronometro);

    if (tiempo==quantum_total)
    {
        //SE INTERRUMPE POR QUANTUM Y LO ENCOLA AL FINAL
        encolarAlFinal(colaRR);
    }
    //else
    //{
    //    if (VERIFICAR SI LA INTERRUPCION ES POR I/O)
    //    {
    //        encolarAlFinal(colaRR);
    //    }
    //}
}

void encolarColaVRR(Cola*colaREADY,Cola*colaVRR)
{   
    // COLA AUXILIAR
    Cola*colaVRRAux=(Cola*)malloc(sizeof(Cola));
    colaVRRAux->primero=colaVRRAux->ultimo=NULL;

    NodoColaPCBS*puntero;
    puntero=colaVRRAux->primero;

    while (puntero!=NULL)
    {
        NodoColaPCBS*nuevo;
        nuevo=malloc(sizeof(NodoColaPCBS));
        nuevo->PCBS.PID=puntero->PCBS.PID;
        nuevo->PCBS.PC=puntero->PCBS.PC;
        nuevo->PCBS.Quantum=puntero->PCBS.Quantum;
        nuevo->sig=NULL;

        if (colaVRRAux->ultimo!=NULL)
        {
            colaVRRAux->ultimo->sig=nuevo;
        }
        else
        {
            colaVRRAux->primero=nuevo;
        }
        colaVRRAux->ultimo=nuevo;
        puntero=puntero->sig;
    }

    //SE DEBE DESENCOLAR UN PCB Y
    //ENVIAR CONTEXTO DE EJECUCION

    t_temporal* cronometro = temporal_create();
    int quantum_total = puntero->PCBS.Quantum;

    //ESPERO MENSAJE DE CPU Y PARO EL CRONOMETRO

    temporal_stop(cronometro);
    int64_t tiempo = temporal_gettime(cronometro);

    if (tiempo==quantum_total)
    {
        //SE INTERRUMPE POR QUANTUM Y LO ENCOLA AL FINAL
        encolarAlFinal(colaVRR);
    }
    else
    {

        if (VERIFICAR SI LA INTERRUPCION ES POR I/O)
        {
            encolarAlFinal(colaVRR);
        }
        else
        {
            if (tiempo<quantum_total)
            {
                //PASAR EL PCB DEL PRIMER LUGAR DE LA COLA VRR A VRR Auxiliar
                
                NodoColaPCBS*pasarAux;
                pasarAux=colaVRR->primero;
                colaVRR->primero=colaVRR>primero->sig;
                pcb pasaColaAux;
                pasaColaAux.PC = pasarAux->PCBS.PC;
                pasaColaAux.PID = pasarAux->PCBS.PID;
                pasaColaAux.Quantum = (pasarAux->PCBS.Quantum)-tiempo;

                encolarColaVRRAux(pasaColaAux,colaVRRAux);
            }
        }
    }
    
}

void encolarColaVRRAux(pcb ProcesoNuevo,Cola*colaVRRAux)
{   
    NodoColaPCBS*nuevo;
    nuevo=malloc(sizeof(NodoColaPCBS));

    nuevo->PCBS.PID=ProcesoNuevo.PID;
    nuevo->PCBS.PC=ProcesoNuevo.PC;
    nuevo->PCBS.Quantum=ProcesoNuevo.Quantum;
    nuevo->sig=NULL;
    if (colaVRRAux->ultimo!=NULL)
    {
        colaVRRAux->ultimo->sig=nuevo;
    }
    else
    {
        colaVRRAux->primero=nuevo;
    }
    colaVRRAux->ultimo=nuevo;
}

void encolarAlFinal(Cola*cola)
{
    NodoColaPCBS*puntero;
    puntero=cola->primero;

    while (puntero!=NULL)
    {
        NodoColaPCBS*nuevo;
        nuevo=malloc(sizeof(NodoColaPCBS));
        nuevo->PCBS.PID=puntero->PCBS.PID;
        nuevo->PCBS.PC=puntero->PCBS.PC;
        nuevo->PCBS.Quantum=puntero->PCBS.Quantum;
        nuevo->sig=NULL;

        if (cola->ultimo!=NULL)
        {
            cola->ultimo->sig=nuevo;
        }
        else
        {
            cola->primero=nuevo;
        }
        cola->ultimo=nuevo;
        puntero=puntero->sig;
    }

    //ELIMINAR EL PCB DEL PRIMER LUGAR
    
    NodoColaPCBS*nuevo_primero;
    NodoColaPCBS*eliminar;
    eliminar=cola->primero;
    cola->primero=cola->primero->sig;
    free(eliminar);
}

void listarProcesos(Cola*cola,char*estado,t_log*logger)
{
    NodoColaPCBS*identificador;
    identificador=cola->primero;
    if (identificador!=NULL)
    {
        log_info(logger,"PROCESOS EN ESTADO %s:",estado);
    }
    else
    {
        log_info(logger,"NO HAY PROCESOS EN ESTADO %s",estado);
    }
    
    
    while (identificador!=NULL)
    {
        int pid = identificador->PCBS.PID;
        log_info(logger,"PID: %d",pid);
        identificador=identificador->sig;
    }
}*/