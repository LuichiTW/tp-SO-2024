#include "main.h"

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
    
    char* algo = readline("> ");
    //se tendria que liberar el especio de memoria usado por los elementos

    //CONSOLA INTERACTIVA
    iniciar_consola_interactiva(logger);
    return 0;
}

void iniciar_consola_interactiva(t_log* logger)
{
    char* leido;
    leido = readline("> ");
    bool validacion_leido;

    while (strcmp(leido,"\0") != 0)
    {
        validacion_leido= validacion_de_instruccion_de_consola(leido, logger);
        if (!validacion_leido)
        {
            log_error (logger,"Comando de CONSOLA no reconocido");
            free(leido);
            leido = readline(">");
            continue; //Saltar y continuar con el resto de la iteracion
        }
        //atender_instruccion_valida(leido);
        free(leido);
        leido = readline("> ");
    }
    free(leido);
}

bool validacion_de_instruccion_de_consola(char* leido, t_log* logger)
{
    char** comando_consola = string_split(leido, " ");

    //HABRIA QUE AGREGAR MAS VALIDACIONES, COMO EN LOS PARAMETROS

    bool resultado_validacion = false;
    if (strcmp(comando_consola[0], "EJECUTAR_SCRIPT") == 0)
    {
        resultado_validacion = true;
    }
    else if(strcmp(comando_consola[0], "INICIAR_PROCESO") == 0)
    {
        resultado_validacion = true;
    }
    else if(strcmp(comando_consola[0], "FINALIZAR_PROCESO") == 0)
    {
        resultado_validacion = true;
    }
    else if(strcmp(comando_consola[0], "DETENER_PLANIFICACION") == 0)
    {
        resultado_validacion = true;
    }
    else if(strcmp(comando_consola[0], "INICIAR_PLANIFICACION") == 0)
    {
        resultado_validacion = true;
    }
    else if(strcmp(comando_consola[0], "MULTIPROGRAMACION") == 0)
    {
        resultado_validacion = true;
    }
    else if(strcmp(comando_consola[0], "PROCESO_ESTADO") == 0)
    {
        resultado_validacion = true;
    }else
    {
        log_error(logger,"Comando no reconocido.");
        resultado_validacion = false;
    }
    string_array_destroy(comando_consola);
    return resultado_validacion;
}