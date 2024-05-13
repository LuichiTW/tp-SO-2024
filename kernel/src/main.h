#ifndef MAIN_KERNEL
#define MAIN_KERNEL

#include <stdlib.h>
#include <stdio.h>
#include <utils/conexiones/conexiones_servidor.h>
#include <utils/conexiones/conexiones_cliente.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/error.h>
#include <readline/readline.h>
#include <pthread.h>
#include <string.h>

void iniciar_consola_interactiva(t_log* logger);
bool validacion_de_instruccion_de_consola(char* leido, t_log* logger);

#endif // !MAIN_KERNEL
