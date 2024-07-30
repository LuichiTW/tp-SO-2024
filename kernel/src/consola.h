#ifndef CONSOLA_H
#define CONSOLA_H

#include <stdlib.h>
#include <readline/readline.h>
#include <commons/string.h>
#include <commons/log.h>

enum comandos_consola {
    COM_EJECUTAR_SCRIPT,
    COM_INICIAR_PROCESO,
    COM_FINALIZAR_PROCESO,
    COM_DETENER_PLANIFICACION,
    COM_INICIAR_PLANIFICACION,
    COM_MULTIPROGRAMACION,
    COM_PROCESO_ESTADO
};

void iniciar_consola();
// Traduce a int el comando pasado por parámetro, o devuelve -1 en caso de que no exista.
int traducir_comando(char *comando);

// Funciones de comandos

void c_ejecutar_script(char *path);
void c_iniciar_proceso(char *path);
void c_finalizar_proceso(int pid);
void c_detener_planificacion();
void c_iniciar_planificacion();
void c_multiprogramacion(int valor);
void c_proceso_estado();

#endif // CONSOLA_H