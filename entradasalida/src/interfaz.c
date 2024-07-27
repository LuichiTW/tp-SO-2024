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

void manejo_config_interfaz(t_config config)
{
    char *tipo_interfaz = config_get_string_value(config, "TIPO_INTERFAZ");
    parametros.logger = logger; //??
    if (tipo_interfaz == NULL)
    {
        perror("Error al obtener el valor de TIPO_INTERFAZ");
        exit(EXIT_FAILURE);
    }
    if (strcmp(tipo_interfaz, "GENERICA") == 0)
    {
        log_info(logger, "Interfaz generica");
        // cargar config de interfaz generica
        config_interfaz.ip_kernel = config_get_string_value(config, "IP_KERNEL");
        config_interfaz.puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");
        config_interfaz.tiempo_unidad_trabajo = config_get_int_value(config, "TIEMPO_UNIDAD_TRABAJO");

        // crear conexiones
        parametros.conexion_kernel = crear_conexion(config_interfaz.ip_kernel, config_interfaz.puerto_kernel, "conexion con IO");
        parametros.server_fd = iniciar_servidor(config_interfaz.puerto_kernel);
    }
    else if (strcmp(tipo_interfaz, "STDIN") == 0)
    {
        log_info(logger, "Interfaz STDIN");
        // cargar config de interfaz stdin
        config_interfaz.ip_memoria = config_get_string_value(config, "IP_MEMORIA");
        config_interfaz.puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
        config_interfaz.ip_kernel = config_get_string_value(config, "IP_KERNEL");
        config_interfaz.puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");

        // crear conexiones
        parametros.conexion_memoria = crear_conexion(config_interfaz.ip_memoria, config_interfaz.puerto_memoria, "conexion con IO");
        parametros.conexion_kernel = crear_conexion(config_interfaz.ip_kernel, config_interfaz.puerto_kernel, "conexion con IO");
        parametros.server_fd = iniciar_servidor(config_interfaz.puerto_kernel);
    }
    else if (strcmp(tipo_interfaz, "STDOUT") == 0)
    {
        log_info(logger, "Interfaz STDOUT");
        // cargar config de interfaz stdout
        config_interfaz.ip_memoria = config_get_string_value(config, "IP_MEMORIA");
        config_interfaz.puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
        config_interfaz.ip_kernel = config_get_string_value(config, "IP_KERNEL");
        config_interfaz.puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");

        // crear conexiones
        parametros.conexion_memoria = crear_conexion(config_interfaz.ip_memoria, config_interfaz.puerto_memoria, "conexion con IO");
        parametros.conexion_kernel = crear_conexion(config_interfaz.ip_kernel, config_interfaz.puerto_kernel, "conexion con IO");
        parametros.server_fd = iniciar_servidor(config_interfaz.puerto_kernel);
    }
    else if (strcmp(tipo_interfaz, "DIALFS") == 0)
    {
        log_info(logger, "Interfaz DIALFS");
        // cargar config de interfaz dialfs
        config_interfaz.tiempo_unidad_trabajo = config_get_int_value(config, "TIEMPO_UNIDAD_TRABAJO");
        config_interfaz.path_base_dialfs = config_get_string_value(config, "PATH_BASE_DIALFS");
        config_interfaz.block_size = config_get_int_value(config, "BLOCK_SIZE");
        config_interfaz.block_count = config_get_int_value(config, "BLOCK_COUNT");
        config_interfaz.retraso_compactacion = config_get_int_value(config, "RETRASO_COMPACTACION");
        config_interfaz.ip_memoria = config_get_string_value(config, "IP_MEMORIA");
        config_interfaz.puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
        config_interfaz.ip_kernel = config_get_string_value(config, "IP_KERNEL");
        config_interfaz.puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");

        // crear conexiones
        parametros.conexion_memoria = crear_conexion(config_interfaz.ip_memoria, config_interfaz.puerto_memoria, "conexion con IO");
        parametros.conexion_kernel = crear_conexion(config_interfaz.ip_kernel, config_interfaz.puerto_kernel, "conexion con IO");
        parametros.server_fd = iniciar_servidor(config_interfaz.puerto_kernel);

        // comprobar filesystem
        rutina_filesystem();
    }
    else
    {
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
    int direcciones[sizeof(leer_array_entero(buffer, &desp))];
    memcpy(direcciones, leer_array_entero(buffer, &desp), sizeof(direcciones));

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
    eliminar_paquete(paquete);

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
    int direcciones[sizeof(leer_array_entero(buffer, &desp))];
    memcpy(direcciones, leer_array_entero(buffer, &desp), sizeof(direcciones));
    if (direcciones == NULL)
    {
        return 1;
    }
    int direccionesT[sizeof(direcciones)];
    desp = 0;
    for (int i = 0; i < sizeof(direcciones) + 1; i++)
    { // envia a memoria cada direccion con su respectivo tamaño a leer
        t_paquete *paquete = crear_paquete();
        agregar_a_paquete(paquete, direcciones[i], __SIZEOF_INT__);
        agregar_a_paquete(paquete, tamanio_a_escribir[i], __SIZEOF_INT__);
        enviar_paquete(paquete, parametros.conexion_memoria);

        int socketCliente = esperar_cliente(parametros.conexion_memoria, parametros.logger);
        buffer = recibir_buffer(&size, socketCliente);
        direccionesT[i] = leer_string(buffer, &desp);
        eliminar_paquete(paquete);
    }
    if (direccionesT == NULL)
    {
        return 1;
    }
    size_t num_direcciones = sizeof(direccionesT) / sizeof(direccionesT[0]);

    for (size_t i = 0; i < num_direcciones; ++i)
    {
        char *direccionT = (char *)direccionesT[i];
        strncpy(texto, direccionT, &size);
        printf(texto);
    }

    free(texto);
    free(buffer);
    free(direcciones);
    free(direccionesT);
    return 0;
}

int iO_FS_CREATE(t_parametroEsperar parametros)
{
    int size;
    char *buffer;
    int desp = 0;

    buffer = recibir_buffer(&size, parametros.socket_cliente);
    int pid = leer_entero(buffer, &desp);
    char *nombre_archivo = leer_string(buffer, &desp);
    FILE *bitmap_f = fopen("..\fileSystem\bitmap.dat", "w"); // ruta bitmap

    int i;
    while ((bitarray_test_bit(bitmap_f, i) != 0) && (i < config_dialfs.block_count))
    {
        i++;
    }
    if (bitarray_test_bit(bitmap_f, i) == 0)
    {
        bitarray_set_bit(bitmap_f, i);
        FILE *f = fopen(terminacion_archivo(nombre_archivo, ".txt"), "w");
        fclose(f);
        log_info(parametros.logger, "PID: %d - Operacion: IO_FS_CREATE - Crear Archivo: %d", pid, nombre_archivo);
    }
    else
    {
        return 1;
    }

    fclose(bitmap_f);
    nanosleep(&tiempo, NULL);
    free(buffer);
    free(nombre_archivo);
    crear_metadata(nombre_archivo, i);
    return 0;
}

int iO_FS_DELETE(t_parametroEsperar parametros)
{
    int size;
    char *buffer;
    int desp = 0;

    buffer = recibir_buffer(&size, parametros.socket_cliente);
    int pid = leer_entero(buffer, &desp);
    char *nombre_archivo = leer_string(buffer, &desp);

    int comienzo_archivo = info_archivo(nombre_archivo, "COMIENZO");
    int tamanio_archivo = info_archivo(nombre_archivo, "TAMANIO");

    FILE *bitmap_f = fopen("..\fileSystem\bitmap.dat", "w"); // ruta bitmap

    int i;
    for (i = comienzo_archivo; i < division_redondeada(tamanio_archivo, config_dialfs.block_size); i++)
    { // desde el bloque inicial limpia los bits del bitmap hasta que alcance todos los bloques que ocupa el archivo
        bitarray_clean_bit(bitmap_f, i);
    }

    log_info(parametros.logger, "PID: %d - Operacion: IO_FS_DELETE - Eliminar Archivo: %d", pid, nombre_archivo);
    fclose(bitmap_f);
    remove(terminacion_archivo(nombre_archivo, ".txt"));
    remove(terminacion_archivo(nombre_archivo, "_metadata.txt"));
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
    int pid = leer_entero(buffer, &desp);
    char *nombre_archivo = leer_string(buffer, &desp);
    int tamanio = leer_entero(buffer, &desp);

    truncate(terminacion_archivo(nombre_archivo, ".txt"), tamanio);
    modificar_metadata(nombre_archivo, "TAMANIO", tamanio);

    log_info(parametros.logger, "PID: %d - Operacion: IO_FS_TRUNCATE- Truncar Archivo: %d", pid, nombre_archivo);

    nanosleep(&tiempo, NULL);
    free(buffer);
    free(nombre_archivo);
    return 0;
}

int iO_FS_READ(t_parametroEsperar parametros)
{
    int size;
    char *buffer;
    int desp = 0;

    buffer = recibir_buffer(&size, parametros.socket_cliente);
    int pid = leer_entero(buffer, &desp);
    char *nombre_archivo = terminacion_archivo(leer_string(buffer, &desp), ".txt"); // VER
    int tamanio_a_leer = leer_entero(buffer, &desp);
    int puntero = leer_entero(buffer, &desp);
    char caracter;

    FILE *f = fopen(nombre_archivo, "r");

    if (f == NULL)
    {
        printf("ERROR al abrir el archivo");
        return 1;
    }
    else
    {
        for (int i = puntero; i < puntero + tamanio_a_leer; i++)
        {
            fseek(nombre_archivo, puntero, SEEK_SET);
            caracter = fgetc(f);
            printf("%c", caracter);
        }
    }
    log_info(parametros.logger, "PID: %d - Operacion: IO_FS_READ - Leer Archivo: %s - Tamaño a Leer: %d - Puntero Archivo: %d",
             pid, nombre_archivo, tamanio_a_leer, puntero);

    fclose(f);
    nanosleep(&tiempo, NULL);
    free(buffer);
    free(nombre_archivo);
    return 0;
}

int iO_FS_WRITE(t_parametroEsperar parametros)
{
    int size;
    char *buffer;
    int desp = 0;

    buffer = recibir_buffer(&size, parametros.socket_cliente);
    int pid = leer_entero(buffer, &desp);
    char *nombre_archivo = terminacion_archivo(leer_string(buffer, &desp), ".txt"); // VER
    int tamanio_a_escribir[sizeof(leer_array_entero(buffer, &desp))];
    memcpy(tamanio_a_escribir, leer_array_entero(buffer, &desp), sizeof(tamanio_a_escribir));
    int puntero = leer_entero(buffer, &desp);
    int direcciones[sizeof(leer_array_entero(buffer, &desp))];
    memcpy(direcciones, leer_array_entero(buffer, &desp), sizeof(direcciones));

    if (direcciones == NULL)
    {
        return 1;
    }
    desp = 0;
    FILE *f = fopen(nombre_archivo, "w");
    fseek(nombre_archivo, puntero, SEEK_SET);
    for (int i = 0; i < sizeof(direcciones) + 1; i++)
    { // envia a memoria cada direccion con su respectivo tamaño a leer
        t_paquete *paquete = crear_paquete();
        agregar_a_paquete(paquete, direcciones[i], __SIZEOF_INT__);
        agregar_a_paquete(paquete, tamanio_a_escribir[i], __SIZEOF_INT__);
        enviar_paquete(paquete, parametros.conexion_memoria);

        int socketCliente = esperar_cliente(parametros.conexion_memoria, parametros.logger);
        buffer = recibir_buffer(&size, socketCliente);
        char *texto = leer_string(buffer, &desp);
        fwrite(texto, sizeof(char), strlen(texto), nombre_archivo);
        eliminar_paquete(paquete);
    }

    int tamanio_total = suma_array(tamanio_a_escribir, sizeof(tamanio_a_escribir));

    log_info(parametros.logger, "PID: %d - Operacion: IO_FS_WRITE - Escribir Archivo: %s - Tamaño a Escribir: %d - Puntero Archivo: %d",
             pid, nombre_archivo, tamanio_total, puntero);

    fclose(f);
    nanosleep(&tiempo, NULL);
    free(buffer);
    free(nombre_archivo);
    free(tamanio_a_escribir);
    free(direcciones);
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

int *leer_array_entero(char *buffer, int *desp)
{
    int len = leer_entero(buffer, desp);

    int *arr = malloc((len + 1) * sizeof(int));
    for (int i = 0; i < len; i++)
    {
        arr[i] = leer_entero(buffer, desp);
    }
    arr[len] = -1;

    return arr;
}

void crear_metadata(char *nombre_archivo, int pos)
{
    t_config *metadata = config_create(terminacion_archivo(nombre_archivo, "_metadata.txt"));
    config_set_value(metadata, "COMIENZO", string_itoa(pos));
    config_set_value(metadata, "TAMANIO", "1");
    config_destroy(metadata);
}

void modificar_metadata(char *nombre_archivo, char *parametro, int dato_modificar)
{
    t_config *metadata = config_create(terminacion_archivo(nombre_archivo, "_metadata.txt"));
    config_set_value(metadata, parametro, string_itoa(dato_modificar));
    config_destroy(metadata);
}

int info_archivo(char *nombre_archivo, char *parametro)
{
    t_config *metadata = config_create(terminacion_archivo(nombre_archivo, "_metadata.txt"));
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

char terminacion_archivo(char *archivo, char *terminacion)
{
    size_t nuevo_tamano = strlen(archivo) + strlen(terminacion) + 1; // +1 para el carácter nulo
    char *nuevo_archivo = (char *)malloc(nuevo_tamano);
    // Copiar el nombre del archivo original y agregar la terminación
    strcpy(nuevo_archivo, archivo);
    strcat(nuevo_archivo, terminacion);

    return nuevo_archivo;
}

int suma_array(int *array, int tamanio)
{
    int suma = 0;
    for (int i = 0; i < tamanio; i++)
    {
        suma += array[i];
    }
    return suma;
}
