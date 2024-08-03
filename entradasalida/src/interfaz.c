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

int obtener_tipo_interfaz(char *tipo_interfaz)
{
    if (strcmp(tipo_interfaz, "GENERICA") == 0)
    {
        return GENERICA;
    }
    else if (strcmp(tipo_interfaz, "STDIN") == 0)
    {
        return STDIN;
    }
    else if (strcmp(tipo_interfaz, "STDOUT") == 0)
    {
        return STDOUT;
    }
    else if (strcmp(tipo_interfaz, "DIALFS") == 0)
    {
        return DIALFS;
    }
    else
    {
        perror("Error al obtener el valor de TIPO_INTERFAZ");
        exit(EXIT_FAILURE);
    }
}

void manejo_config_interfaz(t_config *config)
{
    config_interfaz->tipo_interfaz = config_get_string_value(config, "TIPO_INTERFAZ");
    parametros.logger = logger; //??
    
    if (config_interfaz->tipo_interfaz == NULL)
    {
        perror("Error al obtener el valor de TIPO_INTERFAZ");
        exit(EXIT_FAILURE);
    }

    switch (obtener_tipo_interfaz(config_interfaz->tipo_interfaz))
    {
    case GENERICA:
        log_info(logger, "Interfaz generica");
        // cargar config de interfaz generica
        config_interfaz->ip_kernel = config_get_string_value(config, "IP_KERNEL");
        config_interfaz->puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");
        config_interfaz->tiempo_unidad_trabajo = config_get_int_value(config, "TIEMPO_UNIDAD_TRABAJO");

        // crear conexiones
        parametros.conexion_kernel = crear_conexion(config_interfaz->ip_kernel, config_interfaz->puerto_kernel, "conexion con IO");
        parametros.server_fd = iniciar_servidor(config_interfaz->puerto_kernel);
        break;
    case STDIN:
        log_info(logger, "Interfaz STDIN");
        // cargar config de interfaz stdin
        config_interfaz->ip_memoria = config_get_string_value(config, "IP_MEMORIA");
        config_interfaz->puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
        config_interfaz->ip_kernel = config_get_string_value(config, "IP_KERNEL");
        config_interfaz->puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");

        // crear conexiones
        parametros.conexion_memoria = crear_conexion(config_interfaz->ip_memoria, config_interfaz->puerto_memoria, "conexion con IO");
        parametros.conexion_kernel = crear_conexion(config_interfaz->ip_kernel, config_interfaz->puerto_kernel, "conexion con IO");
        parametros.server_fd = iniciar_servidor(config_interfaz->puerto_kernel);
        break;
    case STDOUT:
        log_info(logger, "Interfaz STDOUT");
        // cargar config de interfaz stdout
        config_interfaz->ip_memoria = config_get_string_value(config, "IP_MEMORIA");
        config_interfaz->puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
        config_interfaz->ip_kernel = config_get_string_value(config, "IP_KERNEL");
        config_interfaz->puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");

        // crear conexiones
        parametros.conexion_memoria = crear_conexion(config_interfaz->ip_memoria, config_interfaz->puerto_memoria, "conexion con IO");
        parametros.conexion_kernel = crear_conexion(config_interfaz->ip_kernel, config_interfaz->puerto_kernel, "conexion con IO");
        parametros.server_fd = iniciar_servidor(config_interfaz->puerto_kernel);
        break;
    case DIALFS:
        log_info(logger, "Interfaz DIALFS");
        // cargar config de interfaz dialfs
        config_interfaz->tiempo_unidad_trabajo = config_get_int_value(config, "TIEMPO_UNIDAD_TRABAJO");
        config_interfaz->path_base_dialfs = config_get_string_value(config, "PATH_BASE_DIALFS");
        config_interfaz->block_size = config_get_int_value(config, "BLOCK_SIZE");
        config_interfaz->block_count = config_get_int_value(config, "BLOCK_COUNT");
        config_interfaz->retraso_compactacion = config_get_int_value(config, "RETRASO_COMPACTACION");
        config_interfaz->ip_memoria = config_get_string_value(config, "IP_MEMORIA");
        config_interfaz->puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
        config_interfaz->ip_kernel = config_get_string_value(config, "IP_KERNEL");
        config_interfaz->puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");

        // crear conexiones
        parametros.conexion_memoria = crear_conexion(config_interfaz->ip_memoria, config_interfaz->puerto_memoria, "conexion con IO");
        parametros.conexion_kernel = crear_conexion(config_interfaz->ip_kernel, config_interfaz->puerto_kernel, "conexion con IO");
        parametros.server_fd = iniciar_servidor(config_interfaz->puerto_kernel);

        // comprobar filesystem
        comprobar_filesystem(config_interfaz);
        break;    
    default:
        perror("Error al obtener el valor de TIPO_INTERFAZ");
        exit(EXIT_FAILURE);
        break;
    }
}

//! realizar unit testing para cada funcion de interfaz

int iO_GEN_SLEEP(t_parametroEsperar parametros)
{
    int size;
    char *buffer;
    int desp = 0;

    buffer = recibir_buffer(&size, parametros.socket_cliente);
    int pid = leer_entero(buffer, &desp);

    int uTrabajo = leer_entero(buffer, &desp);
    for (int i = 0; i < uTrabajo; i++)
    {
        nanosleep(&tiempo, NULL);
    }

    log_info(parametros.logger, "PID: %d - Operacion: IO_GEN_SLEEP", pid );

    free(buffer);
    return 0;
}

int iO_STDIN_READ(t_parametroEsperar parametros)
{
    int size;
    char *buffer;
    int desp = 0;
    char *texto;

    buffer = recibir_buffer(&size, parametros.socket_cliente);
    int pid = leer_entero(buffer, &desp);
    
    int numero_direcciones = leer_entero(buffer,&desp);
    char **direcciones[numero_direcciones];
    memcpy(direcciones, leer_array_string(buffer, &desp), numero_direcciones);

    int *tamanios[numero_direcciones];
    memcpy(tamanios, leer_array_string(buffer, &desp), numero_direcciones);

    texto = readline("> ");
    int anterior = 0;

    for (int i = 0; i < numero_direcciones; i++)
    {
        t_paquete *paquete = crear_paquete();
        agregar_a_paquete(paquete, direcciones[i], __SIZEOF_INT__);
        agregar_a_paquete(paquete,tamanios[i],__SIZEOF_INT__);
        agregar_a_paquete(paquete, leer_subcadena(texto,anterior,*tamanios[i]), *tamanios[i] + 1);


        enviar_peticion(paquete, parametros.conexion_memoria,MEM_ESCRIBIR_MEMORIA);
        eliminar_paquete(paquete);
        anterior = *tamanios[i];
    }
    log_info(parametros.logger, "PID: %d - Operacion: IO_STDIN_READ", pid );

    free(texto);
    free(buffer);
    return 0;
}

int iO_STDOUT_WRITE(t_parametroEsperar parametros)
{
    int size;
    char *buffer;
    int desp = 0;

    buffer = recibir_buffer(&size, parametros.socket_cliente);
    int pid = leer_entero(buffer, &desp);
    
    int numero_direcciones = leer_entero(buffer,&desp);
    char *direcciones[numero_direcciones];
    memcpy(direcciones, leer_array_string(buffer, &desp), numero_direcciones);

    int *tamanios[numero_direcciones];
    memcpy(tamanios, leer_array_string(buffer, &desp), numero_direcciones);

    char *direccionesT[numero_direcciones];
    desp = 0;
    for (int i = 0; i < sizeof(direcciones) + 1; i++)
    { // envia a memoria cada direccion con su respectivo tamaño a leer
        t_paquete *paquete = crear_paquete();
        agregar_a_paquete(paquete, direcciones[i], __SIZEOF_INT__);
        agregar_a_paquete(paquete, tamanios[i], __SIZEOF_INT__);
        enviar_peticion(paquete, parametros.conexion_memoria,MEM_LEER_MEMORIA);

        int socketCliente = esperar_cliente(parametros.conexion_memoria, parametros.logger);
        buffer = recibir_buffer(&size, socketCliente);
        direccionesT[i] = leer_string(buffer, &desp);
        eliminar_paquete(paquete);
    }

    for (int i = 0; i < numero_direcciones; ++i)
    {
        printf("%s",direccionesT[i]);
    }
    log_info(parametros.logger, "PID: %d - Operacion: IO_STDOUT_WRITE", pid );
    
    free(buffer);
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

    // crear archivo
    int pos = crear_archivo_bloques(bloques);

    // actualizar bitmap
    actualizar_bitmap(bitmap, bloques);

    // crear metadata
    crear_metadata(nombre_archivo, pos);

    log_info(parametros.logger, "PID: %d - Operacion: IO_FS_CREATE - Crear Archivo: %s", pid, nombre_archivo);

    nanosleep(&tiempo, NULL);
    free(buffer);
    free(nombre_archivo);
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
    int bloque_inicial = info_archivo(nombre_archivo, "BLOQUE_INICIAL");
    int tamanio_archivo = info_archivo(nombre_archivo, "TAMANIO");

    // eliminar bloques
    eliminar_archivo_bloques(bloques, bloque_inicial,(division_redondeada(tamanio_archivo,config_interfaz->block_size))); // todo falta inplementar

    // eliminar archivo
    actualizar_bitmap(bitmap,bloques);

    log_info(parametros.logger, "PID: %d - Operacion: IO_FS_DELETE - Eliminar Archivo: %s", pid, nombre_archivo);
    // remove(nombre_archivo);
    remove(terminacion_archivo("metadata_", nombre_archivo));
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
    int tamanio = leer_entero(buffer, &desp); // tamaño a aumentar o disminuir
    int tamanio_archivo = info_archivo(nombre_archivo, "TAMANIO");
    
    log_info(parametros.logger, "PID: %d - Operacion: IO_FS_TRUNCATE- Truncar Archivo: %s", pid, nombre_archivo);
    log_info(parametros.logger, "PID: %d - Inicio Compactación." , pid);
    if (tamanio < 0) //que hacer en caso de tamanio 0
    {
        //truncate(nombre_archivo, tamanio);
        
        //cambiar el tamaño del archivo
        cambiar_seccion_lista(bloques, info_archivo(nombre_archivo, "BLOQUE_INICIAL") + division_redondeada(tamanio,config_interfaz->block_size), division_redondeada(tamanio,config_interfaz->block_size), "");

        actualizar_bitmap(bitmap,bloques);
        modificar_metadata(nombre_archivo, "TAMANIO", tamanio);
        compactacion(bloques, bitmap);
    }
    else
    {
        //sacar archivo de bloques
        t_bloque *aux = extraer_parte_lista(bloques, info_archivo(nombre_archivo, "BLOQUE_INICIAL"), division_redondeada(tamanio,config_interfaz->block_size));

        // compactacion
        compactacion(bloques, bitmap);
        //agregar archivo al final de bloques
        insertar_archivo_bloques(aux);

        actualizar_bitmap(bitmap,bloques);
        modificar_metadata(nombre_archivo, "TAMANIO", tamanio + tamanio_archivo);
    }


    nanosleep(&tiempo, NULL);
    log_info(parametros.logger, "PID: %d - Fin Compactación." , pid);
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
    char *nombre_archivo = leer_string(buffer, &desp);
    int puntero = leer_entero(buffer, &desp);
    int numero_direcciones = leer_entero(buffer, &desp);

    char *direcciones[numero_direcciones];
    memcpy(direcciones, leer_array_string(buffer, &desp), numero_direcciones);

    int *tamanios[numero_direcciones];
    memcpy(tamanios,convertir_strings_enteros(leer_array_string(buffer, &desp),numero_direcciones),numero_direcciones);
    


    int comienzo_archivo = info_archivo(nombre_archivo,"BLOQUE_INICIAL");

    char *texto = NULL;
    char *path_bloques = string_from_format("%s%s", config_interfaz->path_base_dialfs, "/bloques.dat");
    FILE *f = fopen(path_bloques, "r");
    int anterior = 0;
    for (int i = 0; i < numero_direcciones; i++)
    {
        fseek(f, anterior ,(comienzo_archivo*config_interfaz->block_size) + puntero );
        for(int j=0;j< *tamanios[i];j++){
            fseek(f, *tamanios[i] ,(comienzo_archivo*config_interfaz->block_size) + puntero + anterior );
            agregar_caracter(texto, fgetc(f));
        }

        t_paquete *paquete = crear_paquete();
        agregar_a_paquete(paquete, direcciones[i], __SIZEOF_INT__);
        agregar_a_paquete(paquete,tamanios[i]+1,__SIZEOF_INT__);
        agregar_a_paquete(paquete, texto, *tamanios[i] + 1);

        enviar_peticion(paquete, parametros.conexion_memoria, MEM_ESCRIBIR_MEMORIA);
        eliminar_paquete(paquete);

        anterior += *tamanios[i];
        texto = NULL;
    }

    int tamanio_a_leer = suma_array(*tamanios,numero_direcciones);

    log_info(parametros.logger, "PID: %d - Operacion: IO_FS_READ - Leer Archivo: %s - Tamaño a Leer: %d - Puntero Archivo: %d",
             pid, nombre_archivo, tamanio_a_leer, puntero);

    fclose(f);
    nanosleep(&tiempo, NULL);
    free(buffer);
    free(nombre_archivo);
    free(path_bloques);
    free(texto);
    return 0;
}

int iO_FS_WRITE(t_parametroEsperar parametros)
{
    int size;
    char *buffer;
    int desp = 0;

    buffer = recibir_buffer(&size, parametros.socket_cliente);
    int pid = leer_entero(buffer, &desp);
    char *nombre_archivo = leer_string(buffer, &desp);

    int numero_direcciones = leer_entero(buffer,&desp);

    int *tamanio_a_escribir[numero_direcciones];
    memcpy(tamanio_a_escribir,convertir_strings_enteros(leer_array_string(buffer, &desp),numero_direcciones),numero_direcciones);
    int puntero = leer_entero(buffer, &desp);

    char *direcciones[numero_direcciones];
    memcpy(direcciones, leer_array_string(buffer, &desp), numero_direcciones);

    int comienzo_archivo = info_archivo(nombre_archivo,"BLOQUE_INICIAL");
    desp = 0;

    char *path_bloques = string_from_format("%s%s", config_interfaz->path_base_dialfs, "/bloques.dat");
    FILE *f = fopen(path_bloques, "w");

    int anterior = 0;

    for (int i = 0; i < numero_direcciones; i++)
    { // envia a memoria cada direccion con su respectivo tamaño a leer
        t_paquete *paquete = crear_paquete();
        agregar_a_paquete(paquete, direcciones[i], __SIZEOF_INT__);
        agregar_a_paquete(paquete, tamanio_a_escribir[i] + 1, __SIZEOF_INT__);
        enviar_peticion(paquete, parametros.conexion_memoria,MEM_LEER_MEMORIA);

        int socketCliente = esperar_cliente(parametros.conexion_memoria, parametros.logger);
        buffer = recibir_buffer(&size, socketCliente);
        char *texto = leer_string(buffer, &desp); 

        fseek(f, anterior, (comienzo_archivo*config_interfaz->block_size) + puntero);
        fwrite(texto, sizeof(char), strlen(texto), f);
        eliminar_paquete(paquete);
        anterior += *tamanio_a_escribir[i];
    }

    int tamanio_total = suma_array(*tamanio_a_escribir, numero_direcciones);

    log_info(parametros.logger, "PID: %d - Operacion: IO_FS_WRITE - Escribir Archivo: %s - Tamaño a Escribir: %d - Puntero Archivo: %d",pid, nombre_archivo, tamanio_total, puntero);

    fclose(f);
    nanosleep(&tiempo, NULL);
    free(buffer);
    free(nombre_archivo);
    free(path_bloques);
    return 0;
}

int leer_entero(char *buffer, int *desplazamiento)
{
    int ret;
    memcpy(&ret, buffer + (*desplazamiento), sizeof(int));
    (*desplazamiento) += sizeof(int);
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
    t_metadata *nuevo = malloc(sizeof(t_metadata));

    // crear archivo metadata
    FILE *f = fopen(terminacion_archivo("metadata_", nombre_archivo), "w");
    fclose(f);

    // agregar datos al archivo metadata
    t_config *metadata = config_create(terminacion_archivo("metadata_", nombre_archivo));
    nuevo->nombre = nombre_archivo;
    nuevo->bloque_inicial = pos;
    nuevo->siguiente = NULL;
    insertar_a_lista(nuevo);

    config_set_value(metadata, "BLOQUE_INICIAL", string_itoa(pos));
    config_set_value(metadata, "TAMANIO_ARCHIVO", "1");
    config_destroy(metadata);
}


int info_archivo(char *nombre_archivo, char *parametro)
{
    char *path_metadata = terminacion_archivo("metadata_", nombre_archivo);
    path_metadata = string_from_format("%s%s", config_interfaz->path_base_dialfs, path_metadata); 
    t_config *metadata = config_create(path_metadata);
    int info = config_get_int_value(metadata, parametro);
    config_destroy(metadata);
    free(path_metadata);
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


int suma_array(int *array, int tamanio)
{
    int suma = 0;
    for (int i = 0; i < tamanio; i++)
    {
        suma += array[i];
    }
    return suma;
}



char* leer_subcadena(const char* cadena, size_t inicio, size_t fin) {
    if (inicio > fin || inicio >= strlen(cadena)) {
        return NULL; // Verificación básica de parámetros
    }

    size_t longitud = fin - inicio + 1;
    char* subcadena = (char*)malloc((longitud + 1) * sizeof(char));
    if (subcadena == NULL) {
        perror("Error al asignar memoria");
        exit(EXIT_FAILURE);
    }

    strncpy(subcadena, cadena + inicio, longitud);
    subcadena[longitud] = '\0'; // Asegurar la terminación con null

    return subcadena;
}

char* agregar_caracter(const char* texto, char caracter) {
    size_t longitud = strlen(texto);
    char* nuevo_texto = (char*)malloc((longitud + 2) * sizeof(char)); // +2 para el nuevo caracter y '\0'
    
    if (nuevo_texto == NULL) {
        perror("Error al asignar memoria");
        exit(EXIT_FAILURE);
    }

    strcpy(nuevo_texto, texto);
    nuevo_texto[longitud] = caracter; // Añadir el nuevo caracter al final
    nuevo_texto[longitud + 1] = '\0'; // Añadir el terminador nulo

    return nuevo_texto;
}
char** leer_array_string(char*buffer, int* desp)
{
	int len = leer_entero(buffer, desp);
	char** arr = malloc((len+1)*sizeof(char*));

	for(int i = 0; i < len; i++)
	{
		arr[i] = leer_string(buffer, desp);
	}
	arr[len] = NULL;

	return arr;
}
int *convertir_strings_enteros(char **strings,int tamanio){
    int *enteros = (int*)malloc(tamanio *sizeof(int));
    for (int i = 0; i < tamanio; i++) {
        enteros[i] = atoi(strings[i]);
    }
    return enteros;
}