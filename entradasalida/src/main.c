
#include "interfaz.h"

//todo reorganizar las funciones los archivos
t_config_interfaz *config_interfaz;
t_parametroEsperar parametros;
t_bloque *bloques = NULL;
t_bitarray *bitmap = NULL;
t_metadata *metadata = NULL;
t_log *logger;
struct timespec tiempo;

char *nombreInterfaz = "TECLADO";
char *pathInterfaz = "./dialfs.config"; 

// considerar pasar los argumentos de creacion de interfaces por el main y no por la funcion (issue #3709)
int main(){
    //inicia logger y config
    t_log *logger = iniciar_logger_io();

    t_config *config = iniciar_config(pathInterfaz);

    //carga los datos de la interfaz y crea las conxiones por el tipo de interfaz
    manejo_config_interfaz(config);

    //espera las instrucciones de la interfaz
    esperar(parametros);

    //finaliza logger y config
    log_destroy(logger);
    config_destroy(config);
    return 0;
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
    int tipoInterfaz = recibir_operacion(parametros.socket_cliente);
    int resultado;

    switch (tipoInterfaz)
    {
    case IO_GEN_SLEEP:
        resultado = iO_GEN_SLEEP(parametros);
        break;
    case IO_STDIN_READ:

        resultado = iO_STDIN_READ(parametros);
        break;
    case IO_STDOUT_WRITE:
        resultado = iO_STDOUT_WRITE(parametros);
        break;
    case IO_FS_CREATE:
        resultado = iO_FS_CREATE(parametros);
        break;
    case IO_FS_DELETE:
        resultado = iO_FS_DELETE(parametros);
        break;
    case IO_FS_TRUNCATE:
        resultado = iO_FS_TRUNCATE(parametros); 
        break;
    case IO_FS_WRITE:
        resultado = iO_FS_WRITE(parametros);
        break;
    case IO_FS_READ:
        resultado = iO_FS_READ(parametros);
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

void modificar_metadata(char *nombre_archivo, char *parametro, int dato_modificar)
{
    t_config *metadata = config_create(terminacion_archivo("metadata_", nombre_archivo));
    config_set_value(metadata, parametro, string_itoa(dato_modificar));
    config_destroy(metadata);

    if (string_equals_ignore_case(parametro,"BLOQUE_INICIAL"))
    {
        actualizar_comienzo_lista(nombre_archivo, dato_modificar);
    }
}



void insertar_a_lista(t_metadata *nuevo)
{
    t_metadata *aux = metadata;

    while (aux->siguiente != NULL)
    {
        aux = aux->siguiente;
    }
    if (metadata == NULL)
    {
        metadata = nuevo;
    }
    else
    {
        aux->siguiente = nuevo;
    }
}

char *terminacion_archivo(char *archivo, char *terminacion)
{
    size_t nuevo_tamano = strlen(archivo) + strlen(terminacion) + 1; // +1 para el carácter nulo
    char *nuevo_archivo = (char *)malloc(nuevo_tamano);
    // Copiar el nombre del archivo original y agregar la terminación
    strcpy(nuevo_archivo, archivo);
    strcat(nuevo_archivo, terminacion);

    return nuevo_archivo;
}

void actualizar_comienzo_lista(char *nombre_archivo, int posicion)
{
    t_metadata *aux = metadata;

    while ((aux->nombre != nombre_archivo) && (aux != NULL))
    {
        aux = aux->siguiente;
    }
    if (aux->nombre == nombre_archivo)
    {
        aux->bloque_inicial = posicion;
    }
}



