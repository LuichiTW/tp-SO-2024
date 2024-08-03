#include "io.h"

t_list *interfaces;

struct param_pet_io {
    t_interfaz *interfaz;
    int pid;
    op_code op;
    char *datos;
};
void traducir_peticion_io(struct param_pet_io *params);

void iniciar_server_io() {
    sockets.server_io = iniciar_servidor(config_kernel.puerto_escucha);

    interfaces = list_create();

    pthread_t thread_conexiones;
    pthread_create(&thread_conexiones, NULL, (void*)recibir_conexiones_io, NULL);
    pthread_detach(thread_conexiones);
}


void recibir_conexiones_io(int *socket) {
    t_log *logger = alt_kernel_logger();
    int socket_cliente = esperar_cliente2(sockets.server_io, logger);
    log_destroy(logger);
    
    recibir_operacion(socket_cliente);
    t_list *datos_io = recibir_paquete(socket_cliente);

    t_interfaz *io = malloc(sizeof(*io));
    io->nombre = list_get(datos_io, 0);
    io->tipo = *((int*) list_get(datos_io, 1));

    io->socket = socket_cliente;
    io->pid_usando = 0;
    io->cola_peticiones = queue_create();

    list_destroy(datos_io);

    list_add(interfaces, io);
}

void intentar_peticion_io(char *nombre_interfaz, int pid, op_code op, char *datos) {
    t_interfaz *interfaz = buscar_interfaz(nombre_interfaz);
    t_estado_proceso *est = buscar_estado_proceso(pid);
    est->motivo_bloqueo = BLOQUEO_IO;
    
    if (queue_size(interfaz->cola_peticiones) == 0) {
        struct param_pet_io *params = malloc(sizeof(*params));
        params->interfaz = interfaz;
        params->pid = pid;
        params->op = op;
        params->datos = datos;

        pthread_t t;
        pthread_create(&t, NULL, (void*)traducir_peticion_io, (void*) params);
        pthread_detach(t);
    }
    else {
        t_peticion_io *pet = malloc(sizeof(*pet));
        pet->pid = pid;
        pet->op = op;
        pet->datos = datos;
        queue_push(interfaz->cola_peticiones, pet);
    }
}

void traducir_peticion_io (struct param_pet_io *params) {
    struct param_pet_io params2 = *params;
    free(params);
    peticion_io(params2.interfaz, params2.pid, params2.op, params2.datos);
}


void peticion_io(t_interfaz *interfaz, int pid, op_code op, char *datos) {
    t_paquete *paquete = crear_paquete();
    agregar_a_paquete(paquete, &pid, sizeof(pid));
    agregar_a_paquete(paquete, datos, string_length(datos)+1);
    enviar_peticion(paquete, interfaz->socket, op);
    eliminar_paquete(paquete);

    recibir_operacion(interfaz->socket);
    recibir_entero(interfaz->socket);
    desbloquear_proceso(pid);

    if (queue_size(interfaz->cola_peticiones) > 0) {
        t_peticion_io *prox_pet = queue_pop(interfaz->cola_peticiones);
        peticion_io(interfaz, prox_pet->pid, prox_pet->op, prox_pet->datos);
        free(prox_pet);
    }
}

t_interfaz *buscar_interfaz(char *nombre) {
    bool es_el_buscado(t_interfaz *io) {
        return io->nombre == nombre;
    }
    return list_find(interfaces, (void *) es_el_buscado);
}