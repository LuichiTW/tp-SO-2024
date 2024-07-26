#include "main.h"
t_bitarray *bitmap;

// considerar pasar los argumentos de creacion de interfaces por el main y no por la funcion (issue #3709)
int main(char *nombreInterfaz, char *pathInterfaz)
{
    t_log *logger = iniciar_logger_io();

    t_config *config = iniciar_config(pathInterfaz);

    cargar_config_interfaz(config);

    /* prueba de lectura de archivo de configuracion
    t_config *config = config_create("./io.config");
    if (config == NULL)
    {
        perror("Error al iniciar config");
        exit(EXIT_FAILURE);
    }
    // memoria
    char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    // kernel
    char *ip_kernel = config_get_string_value(config, "IP_KERNEL");
    char *puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");
    */

    // cargar parametros para cargar al hilo
    // tiempo.tv_sec = tiempo_unidad_trabajo;

    t_parametroEsperar parametros;

    // todo la conexion a memoria y kernel va ser manejada por una funcion
    // cliente a memoria
    //parametros.conexion_memoria = crear_conexion(ip_memoria, puerto_memoria, "Hola soy el IO"); 

    // cliente a kernel
    //parametros.conexion_kernel = crear_conexion(ip_kernel, puerto_kernel, "Hola soy el IO");

    // servidor
    parametros.server_fd = iniciar_servidor(puerto_kernel);
    parametros.logger = logger;

    // crear hilo para manejar espera
    // ? no se usaria si cada interfaz es una terminal diferente
    pthread_t kernel_t;
    int resutado = pthread_create(&kernel_t, NULL, (void *)esperar, (void *)&parametros);
    if (resutado != 0)
    {
        log_error(logger, "Error crear hilo");
        exit(EXIT_FAILURE);
    }
    pthread_join(kernel_t, NULL);
    pthread_detach(kernel_t);

    log_destroy(logger);
    config_destroy(config);
    return 0;
    ///////////////////////////////////////////////////////////////////
}

void esperar(t_parametroEsperar parametros)
{

    while (1)
    {
        parametros.socket_cliente = esperar_cliente(parametros.server_fd, parametros.logger);

        if (parametros.socket_cliente != -1)
        {
            pthread_t t;
            pthread_create(&t, NULL, (void *)manejarConexion, (void *)&parametros);
            pthread_join(t, NULL);
            pthread_detach(t);
        }
    }
}

void manejarConexion(t_parametroEsperar parametros)
{
    // !falta implementar el recibir_operacion
    // ? recibir un char* y apartir de eso cambiar el tipo de operacion
    int tipoInterfaz = recibir_operacion(parametros.socket_cliente);
    int resultado;

    switch (tipoInterfaz)
    {
    case IO_GEN_SLEEP:
        log_info(parametros.logger, "PID: <PID> - Operacion: IO_GEN_SLEEP");
        resultado = iO_GEN_SLEEP(parametros);
        break;
    case IO_STDIN_READ:
        log_info(parametros.logger, "PID: <PID> - Operacion: IO_STDIN_READ");
        resultado = iO_STDIN_READ(parametros);
        break;
    case IO_STDOUT_WRITE:
        log_info(parametros.logger, "PID: <PID> - Operacion: IO_STDOUT_WRITE");
        resultado = iO_STDOUT_WRITE(parametros);
        break;
    case IO_FS_CREATE:
        log_info(parametros.logger, "PID: <PID> - Operacion: IO_FS_CREATE");
        resultado = iO_FS_CREATE(parametros);
        break;
    case IO_FS_DELETE:
        log_info(parametros.logger, "PID: <PID> - Operacion: IO_FS_DELETE");
        resultado = iO_FS_DELETE(parametros);
        break;
    case IO_FS_TRUNCATE:
        log_info(parametros.logger, "PID: <PID> - Operacion: IO_FS_TRUNCATE");
        resultado = iO_FS_TRUNCATE(parametros); 
        break;
    case IO_FS_WRITE:
        int size;
        char *buffer;
        int desp = 0;

        buffer = recibir_buffer(&size, parametros.socket_cliente);
        char *nombre_archivo = terminacion_archivo(leer_string(buffer, &desp),".txt"); //VER
        int *tamanio_a_escribir[sizeof(leer_array_entero(buffer, &desp))];
        memcpy(tamanio_a_escribir, leer_array_entero(buffer, &desp), sizeof(tamanio_a_escribir));
        int puntero = leer_entero(buffer,&desp);

        int tamanio_total = suma_array(tamanio_a_escribir,sizeof(tamanio_a_escribir));

        log_info(parametros.logger, "PID: %d - Operacion: IO_FS_WRITE - Escribir Archivo: %s - Tamaño a Escribir: %d - Puntero Archivo: %d", 
        parametros.pid, nombre_archivo, tamanio_total, puntero); //HAY QUE TENER EN CUENTA EL PID EN EL BUFFER

        resultado = iO_FS_WRITE(parametros);
        free(buffer);
        free(nombre_archivo);
        free(tamanio_a_escribir);
        free(puntero);
        break;
    case IO_FS_READ:
        int size;
        char *buffer;
        int desp = 0;

        buffer = recibir_buffer(&size, parametros.socket_cliente);
        char *nombre_archivo = terminacion_archivo(leer_string(buffer, &desp),".txt"); //VER
        int tamanio_a_leer = leer_entero(buffer,&desp);
        memcpy(tamanio_a_escribir, leer_array_entero(buffer, &desp), sizeof(tamanio_a_escribir));
        int puntero = leer_entero(buffer,&desp);


        
        log_info(parametros.logger, "PID: %d - Operacion: IO_FS_WRITE - Escribir Archivo: %s - Tamaño a Escribir: %d - Puntero Archivo: %d", 
        parametros.pid, nombre_archivo, tamanio_a_leer, puntero); //HAY QUE TENER EN CUENTA EL PID EN EL BUFFER

        resultado = iO_FS_READ(parametros);
        free(buffer);
        free(nombre_archivo);
        free(tamanio_a_leer);
        free(puntero);

        break;
        default:
        log_error(parametros.logger, "Tipo de interfaz desconocida: %d", tipoInterfaz);
    }
    if (resultado == 0)
    {
        log_info(parametros.logger, "Operacion exitosa");
        enviar_mensaje("OK", parametros.conexion_kernel);
    }
    else
    {
        enviar_mensaje("ERROR", parametros.conexion_kernel);
    }
}




