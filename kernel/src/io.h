#ifndef SERVER_IO_H
#define SERVER_IO_H

#include <pthread.h>
#include "conexion.h"
#include "planificador.h"

typedef struct {
    int pid;
    op_code op;
    char* datos;
} t_peticion_io;

typedef struct {
    char *nombre;
    int socket;
    enum tipo_interfaz tipo;
    int pid_usando;
    t_queue *cola_peticiones;
} t_interfaz;

extern t_list *interfaces;

void iniciar_server_io();
void recibir_conexiones_io();
void intentar_peticion_io(char *nombre_interfaz, int pid, op_code op, char *datos);
void peticion_io(t_interfaz *interfaz, int pid, op_code op, char *datos);
t_interfaz *buscar_interfaz(char *nombre);

#endif // SERVER_IO_H
