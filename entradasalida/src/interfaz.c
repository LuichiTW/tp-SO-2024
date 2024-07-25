#include "interfaz.h"

// iniciar logger
t_log *iniciar_logger_io(void)
{

    t_log *logger = log_create("io.log", "io", true, LOG_LEVEL_INFO);
    log_info(logger, "Iniciando I/O...");
    if (logger == NULL)
    {
        perror("Error al iniciar logger");
        exit(EXIT_FAILURE);
    }
    return logger;
}

// inicia la configuracion de un archivo de config
t_config *iniciar_config(char *path_config)
{
    t_config *config = config_create(path_config);
    if (config == NULL)
    {
        perror("Error al iniciar config");
        exit(EXIT_FAILURE);
    }
    return config;
}

void cargar_config_interfaz(t_config config){
    char *tipo_interfaz = config_get_string_value(config, "TIPO_INTERFAZ");
    if (tipo_interfaz == NULL) {
        perror("Error al obtener el valor de TIPO_INTERFAZ");
        exit(EXIT_FAILURE);
    }
    if (strcmp(tipo_interfaz, "GENERICA") == 0) {
        log_info(logger, "Interfaz generica");
        //cargar config de interfaz generica
    } else if (strcmp(tipo_interfaz, "STDIN") == 0) {
        log_info(logger, "Interfaz STDIN");
        //cargar config de interfaz stdin
    } else if (strcmp(tipo_interfaz, "STDOUT") == 0) {
        log_info(logger, "Interfaz STDOUT");
        //cargar config de interfaz stdout
    } else if (strcmp(tipo_interfaz, "DIALFS") == 0) {
        log_info(logger, "Interfaz DIALFS");
        //cargar config de interfaz dialfs
    } else {
        perror("Error al obtener el valor de TIPO_INTERFAZ");
        exit(EXIT_FAILURE);
    }
    
}

int iO_GEN_SLEEP(t_parametroEsperar parametros)
{
    int size;
    char *buffer;
    int desp = 0;

    buffer = recibir_buffer(&size, parametros.socket_cliente);

    int uTrabajo = leer_entero(buffer, &desp);
    for (int i = 0; i < uTrabajo; i++)
    {
        nanosleep(&tiempo, NULL);
    }

    free(buffer);
    if (uTrabajo == NULL)
    {
        return 1;
    }
    return 0;
}

int iO_STDIN_READ(t_parametroEsperar parametros)
{
    int size;
    char *buffer;
    int desp = 0;
    char *texto;

    buffer = recibir_buffer(&size, parametros.socket_cliente);
    char direcciones[sizeof(leer_array(buffer, &desp))];
    memcpy(direcciones, leer_array(buffer, &desp), sizeof(direcciones));

    if (direcciones == NULL)
    {
        return 1;
    }
    //! que pasa al tener varios hilos que quieran leer consola
    texto = readline("> ");
    t_paquete *paquete = crear_paquete();
    agregar_a_paquete(paquete, texto, strlen(texto) + 1);
    agregar_a_paquete(paquete, direcciones, sizeof(direcciones) + 1);

    enviar_paquete(paquete, parametros.conexion_memoria);

    free(texto);
    free(buffer);
    return 0;
}

int iO_STDOUT_WRITE(t_parametroEsperar parametros)
{
    int size;
    char *buffer;
    int desp = 0;
    char *texto;

    buffer = recibir_buffer(&size, parametros.socket_cliente);
    char direcciones[sizeof(leer_array(buffer, &desp))];
    memcpy(direcciones, leer_array(buffer, &desp), sizeof(direcciones));
    if (direcciones == NULL)
    {
        return 1;
    }
    t_paquete *paquete = crear_paquete();
    agregar_a_paquete(paquete, direcciones, sizeof(direcciones) + 1);
    enviar_paquete(paquete, parametros.conexion_memoria);

    int socketCliente = esperar_cliente(parametros.conexion_memoria, parametros.logger);
    buffer = recibir_buffer(&size, socketCliente);
    char *direccionesT[sizeof(leer_array(buffer, &desp))];
    memcpy(direccionesT, leer_array(buffer, &desp), sizeof(direccionesT));
    if (direccionesT == NULL)
    {
        return 1;
    }
    size_t num_direcciones = sizeof(direccionesT) / sizeof(direccionesT[0]);

    for (size_t i = 0; i < num_direcciones; ++i)
    {
        char *direccionT = (char *)direccionesT[i];
        strncpy(texto, direccionT, &size);
        printf("Texto leido: %s\n", texto);
    }

    free(texto);
    free(buffer);
    return 0;
}

int iO_FS_CREATE(t_parametroEsperar parametros)
{
    int size;
    char *buffer;
    int desp = 0;

    buffer = recibir_buffer(&size, parametros.socket_cliente);
    char *nombre_archivo = leer_string(buffer, &desp);
    FILE* bitmap_f = fopen("..\fileSystem\bitmap.dat", "w"); //ruta bitmap

    int i;
    for (i = 0; i < config_dialfs.block_count; i++)
    {
        if (bitarray_test_bit(bitmap_f, i) == 0)
        {
            bitarray_set_bit(bitmap_f, i);
            FILE *f = fopen(terminacion_archivo(nombre_archivo,".txt"), "w"); 
            fclose(f);
        }
    }
    fclose(bitmap_f)
    nanosleep(&tiempo, NULL);
    free(buffer);
    free(nombre_archivo);
    crear_metadata(nombre_archivo, i);
    // ACTUALIZAR BITMAP
    return 0;
}

int iO_FS_DELETE(t_parametroEsperar parametros)
{
    int size;
    char *buffer;
    int desp = 0;

    buffer = recibir_buffer(&size, parametros.socket_cliente);
    char *nombre_archivo = leer_string(buffer, &desp);

    int comienzo_archivo = info_archivo(nombre_archivo, "COMIENZO");
    int tamanio_archivo = info_archivo(nombre_archivo, "TAMANIO");

    FILE* bitmap_f = fopen("..\fileSystem\bitmap.dat", "w"); //ruta bitmap

    int i;
    for (i = comienzo_archivo; i < division_redondeada(tamanio_archivo, config_dialfs.block_size); i++)
    { // desde el bloque inicial limpia los bits del bitmap hasta que alcance todos los bloques que ocupa el archivo
        bitarray_clean_bit(bitmap_f, i);
    }
    fclose(bitmap_f);
    remove(terminacion_archivo(nombre_archivo,".txt")); 
    nanosleep(&tiempo, NULL);
    free(buffer);
    free(nombre_archivo);
    return 0;
}

int iO_FS_TRUNCATE(t_parametroEsperar parametros)
{
    int size;
    char *buffer;
    int desp = 0;

    buffer = recibir_buffer(&size, parametros.socket_cliente);
    char *nombre_archivo = leer_string(buffer, &desp);
    int tamanio = leer_entero(buffer, &desp);

    truncate(terminacion_archivo(nombre_archivo,".txt"), tamanio); //HAY QUE VER TRUNCATE
    modificar_metadata(nombre_archivo, "TAMANIO", tamanio);

    nanosleep(&tiempo, NULL);
    free(buffer);
    free(nombre_archivo);
    return 0;
}

int leer_entero(char *buffer, int *desplazamiento)
{
    int ret;
    memcpy(&ret, buffer + (*desplazamiento), sizeof(int));
    (*desplazamiento) += sizeof(int);
    return ret;
}

int leer_64(char *buffer, int *desplazamiento)
{
    int ret;
    memcpy(&ret, buffer + (*desplazamiento), sizeof(int));
    (*desplazamiento) += sizeof(uint64_t);
    return ret;
}

char *leer_string(char *buffer, int *desplazamiento)
{
    int tamanio = leer_entero(buffer, desplazamiento);
    char *valor = malloc(tamanio);
    memcpy(valor, buffer + (*desplazamiento), tamanio);
    (*desplazamiento) += tamanio;
    return valor;
}

char **leer_array(char *buffer, int *desp)
{
    int len = leer_entero(buffer, desp);
    char **arr = malloc((len + 1) * sizeof(char *));
    for (int i = 0; i < len; i++)
    {
        arr[i] = leer_string(buffer, desp);
    }
    arr[len] = NULL;
    return arr;
}

void crear_metadata(char *nombre_archivo, int pos)
{
    t_config *metadata = config_create(terminacion_archivo(nombre_archivo,".txt")); 
    config_set_value(metadata, "COMIENZO", string_itoa(pos));
    config_set_value(metadata, "TAMANIO", "1");
    config_destroy(metadata);
}

void modificar_metadata(char *nombre_archivo, char *parametro, int dato_modificar){
    t_config *metadata = config_create(terminacion_archivo(nombre_archivo,".txt")); 
    config_set_value(metadata, parametro, string_itoa(dato_modificar));
    config_destroy(metadata);
}

int info_archivo(char *nombre_archivo, char *parametro)
{
    t_config *metadata = config_create(terminacion_archivo(nombre_archivo,".txt")); 
    int info = config_get_int_value(metadata, parametro);
    config_destroy(metadata);
    return info;
}
int division_redondeada(int numerador, int denominador)
{
    if (denominador == 0)
    {
        printf("Error: División por cero\n");
        return -1;
    }
    int resultado = numerador / denominador;
    if (numerador % denominador != 0)
    {
        resultado += 1;
    }
    return resultado;
}

char terminacion_archivo(char* archivo,char* terminacion){
    size_t nuevo_tamano = strlen(archivo) + strlen(terminacion) + 1; // +1 para el carácter nulo
    char* nuevo_archivo = (char*)malloc(nuevo_tamano);
    // Copiar el nombre del archivo original y agregar la terminación
    strcpy(nuevo_archivo, archivo);
    strcat(nuevo_archivo, terminacion);
    
    return nuevo_archivo;
}