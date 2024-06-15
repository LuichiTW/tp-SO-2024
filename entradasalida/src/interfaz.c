#include "interfaz.h"

t_interfaz_disponibles interfaces_disponibles[] = {
  { "generica", GENERICA },
  { "stdin", STDIN },
  { "stdout", STDOUT },
  { "dialfs", DIALFS }
};

t_config_generica config_generica;
t_config_stdin config_stdin;
t_config_stdout config_stdout;
t_config_dialfs config_dialfs;

t_log *iniciar_logger_io(void){
    
    t_log *logger = log_create("io.log", "io", true, LOG_LEVEL_INFO);
    log_info(logger, "Iniciando I/O...");
    if(logger == NULL){
        perror("Error al iniciar logger");
        exit(EXIT_FAILURE);
    }
    return logger;
}



void *iniciar_config_io(t_interfaz *interfaces){
    
    t_interfaz *aux = interfaces;
    t_config *config;
    while (aux != NULL)
    {
        if (aux->tipo_de_interfaz == NULL) {
        perror("Error al iniciar config");
        }
    char *tipo_interfaz = aux->tipo_de_interfaz;
    int id = id_tipo_interfaz(tipo_interfaz);
    
    switch (id){
        case GENERICA:
        config = iniciar_config("generica");
        config_generica.tipo_interfaz = config_get_string_value(config, "TIPO_INTERFAZ");
        config_generica.ip_kernel = config_get_string_value(config, "IP_KERNEL");
        config_generica.puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");
        config_generica.tiempo_unidad_trabajo = config_get_int_value(config, "TIEMPO_UNIDAD_TRABAJO");
        break;
        case STDIN:
        config = iniciar_config("stdin");
        config_stdin.tipo_interfaz = config_get_string_value(config, "TIPO_INTERFAZ");
        config_stdin.ip_memoria = config_get_string_value(config, "IP_MEMORIA");
        config_stdin.puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
        config_stdin.ip_kernel = config_get_string_value(config, "IP_KERNEL");
        config_stdin.puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");
        break;
        case STDOUT:
        config = iniciar_config("stdout");
        config_stdout.tipo_interfaz = config_get_string_value(config, "TIPO_INTERFAZ");
        config_stdout.ip_memoria = config_get_string_value(config, "IP_MEMORIA");
        config_stdout.puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
        config_stdout.ip_kernel = config_get_string_value(config, "IP_KERNEL");
        config_stdout.puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");
        break;
        case DIALFS:
        config = iniciar_config("dialfs");
        config_dialfs.tipo_interfaz = config_get_string_value(config, "TIPO_INTERFAZ");
        config_dialfs.tiempo_unidad_trabajo = config_get_int_value(config, "TIEMPO_UNIDAD_TRABAJO");
        config_dialfs.path_base_dialfs = config_get_string_value(config, "PATH_BASE_DIALFS");
        config_dialfs.block_size = config_get_int_value(config, "BLOCK_SIZE");
        config_dialfs.block_count = config_get_int_value(config, "BLOCK_COUNT");
        config_dialfs.retraso_compactacion = config_get_int_value(config, "RETRASO_COMPACTACION");
        config_dialfs.ip_memoria = config_get_string_value(config, "IP_MEMORIA");
        config_dialfs.puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
        config_dialfs.ip_kernel = config_get_string_value(config, "IP_KERNEL");
        config_dialfs.puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");
        break;
    
        default:
        perror("Error al iniciar config");
        exit(EXIT_FAILURE);
        break;
    }
    
        t_interfaz *temp = aux;
        aux = aux->siguiente;
        free(temp);
    }
    
    return config;
}

t_config *iniciar_config(char *tipo_interfaz){
    char *path = string_from_format("./%s.config", tipo_interfaz);
    t_config *config = config_create(path);
    if(config == NULL){
        perror("Error al iniciar config");
        exit(EXIT_FAILURE);
    }
    return config;
}

 t_interfaz *agregar_interfaz( t_interfaz *interfaces, t_interfaz *nueva_interfaz){
    t_interfaz *aux = interfaces;

    // crear nuevo nodo
    t_interfaz *new_node = malloc(sizeof(t_interfaz));
    new_node->nombre = strdup(nueva_interfaz->nombre);
    new_node->tipo_de_interfaz = strdup(nueva_interfaz->tipo_de_interfaz);
    new_node->siguiente = NULL;

    // si esta vacia usarlo como primer nodo
    if (aux == NULL) {
        return new_node;
    }

    // moverse al final de la lista
    while (aux->siguiente != NULL) {
        aux = aux->siguiente;
    }

    // agregar el nuevo nodo al final de la lista
    aux->siguiente = new_node;

    return interfaces;
}

 t_interfaz *crear_interfaces(void){
     t_interfaz *interfaces = malloc(sizeof(t_interfaz));
     if(interfaces == NULL){
        perror("Error al alocar memoria interfaz");
        exit(EXIT_FAILURE);
     }
    char *nuevo_io = readline("Crear io:<nombre_interfaz> <tipo_interfaz>\n");
    char **parametros = string_split(nuevo_io, " ");
    if(!es_tipo_interfaz(parametros[1])){
        free(nuevo_io);
        free(parametros);
        perror("Tipo de interfaz no reconocido");
        exit(EXIT_FAILURE);
    }
    char *nombre_io;
    char *tipo_interfaz;
    // inicializar primer interfaz
    interfaces->nombre = parametros[0];
    interfaces->tipo_de_interfaz = parametros[1];
    interfaces->siguiente = NULL;
    if (interfaces->nombre == NULL || interfaces->tipo_de_interfaz == NULL){
        perror("Error al ingresar parametros");
        exit(EXIT_FAILURE);
    
    }
    while (!string_is_empty(nuevo_io))
    {   
        free(parametros);
        nuevo_io = readline("Crear io:<nombre_interfaz> <tipo_interfaz>\n");

        if(string_is_empty(nuevo_io)){
        free(nuevo_io);
        perror("saliendo del creador de interfaces");
        break;
        }

        parametros = string_split(nuevo_io, " ");
        nombre_io = parametros[0];
        tipo_interfaz = parametros[1];

        //verifica que los parametros no sean nulos
        if ((tipo_interfaz == NULL || nombre_io == NULL) && !string_is_empty(nuevo_io)){
        perror("Error al ingresar parametros");
        free(nuevo_io);
        free(parametros);
        free(tipo_interfaz);
        free(nombre_io);
        exit(EXIT_FAILURE);
        }

        //verifica que el tipo de interfaz sea valido
        if(!es_tipo_interfaz(tipo_interfaz)){
        free(nuevo_io);
        free(parametros);
        free(tipo_interfaz);
        free(nombre_io);
        perror("Tipo de interfaz no reconocido");
        exit(EXIT_FAILURE);
        }

        //agregar nueva interfaz, cargando los datos
        t_interfaz *nueva_interfaz = malloc(sizeof(t_interfaz));
        if (nueva_interfaz == NULL)
        {
            perror("Error al alocar memoria interfaz");
            exit(EXIT_FAILURE);
        }
        nueva_interfaz->nombre = nombre_io;
        nueva_interfaz->tipo_de_interfaz = tipo_interfaz;
        nueva_interfaz->siguiente = NULL;

        //añadir a la lista
        interfaces = agregar_interfaz(interfaces, nueva_interfaz);

        free(nombre_io);
        free(tipo_interfaz);
        perror("interfaz creada con exito");
    }
    
    recorrer_lista_nombres_tipos(interfaces);
    return interfaces;
}


bool es_tipo_interfaz(char *tipo_interfaz){
    for (int i = 0; i < 4; i++)
    {
        if (strcmp(tipo_interfaz, interfaces_disponibles[i].nombre_interfaz) == 0)
        {
            return true;
        }
    }
    return false;
}

int id_tipo_interfaz(char *tipo_interfaz){
    for (int i = 0; i < 4; i++)
    {
        if (strcmp(tipo_interfaz, interfaces_disponibles[i].nombre_interfaz) == 0)
        {
            return i;
        }
    }
    return -1;
}


void recorrer_lista_nombres_tipos(t_interfaz *interfaces){
    t_interfaz *aux = interfaces;
    while (aux != NULL)
    {
        if (aux->nombre != NULL) {
            printf("Nombre: %s\n", aux->nombre);
        }
        if (aux->tipo_de_interfaz != NULL) {
            printf("Tipo de interfaz: %s\n", aux->tipo_de_interfaz);
        }
        t_interfaz *temp = aux;
        aux = aux->siguiente;
        free(temp);
    }

}


    
    /* //!AGREGAR AL FINAL PARA LIBERAR LA LISTA   
    void eliminar_lista(struct Node** head) {
    struct Node* current = *head;
    struct Node* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    *head = NULL; // Marcar la lista como vacía
    }
    */
    
    
