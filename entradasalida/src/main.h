#ifndef MAIN_IO
#define MAIN_IO

#include <stdlib.h>
#include <stdio.h>
#include <utils/conexiones/conexiones_cliente.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/error.h>
#include <pthread.h>
#include <readline/readline.h>
#include <utils/conexiones/conexiones_servidor.h>
#include <commons/string.h>
#include <commons/bitarray.h>

#include "interfaz.h"

struct timespec tiempo;

typedef struct
{
	int conexion_memoria;
	int conexion_kernel;
	int server_fd;
    int socket_cliente;
	t_log *logger;
} t_parametroEsperar;

enum tipo_interfaz_instruccion
{
    IO_GEN_SLEEP,
    IO_STDIN_READ,
    IO_STDOUT_WRITE
};

//extern t_config_dialfs config_dialfs; 

int iO_GEN_SLEEP(t_parametroEsperar parametros);
int iO_STDIN_READ(t_parametroEsperar parametros);
int iO_STDOUT_WRITE(t_parametroEsperar parametros);
int iO_FS_CREATE(t_parametroEsperar parametros);
int iO_FS_DELETE(t_parametroEsperar parametros);
int iO_FS_TRUNCATE(t_parametroEsperar parametros);

int leer_entero(char*buffer, int* desplazamiento);
int leer_64(char*buffer, int* desplazamiento);

char* leer_string(char* buffer, int* desplazamiento);
char** leer_array(char *buffer, int* desp);

void manejarConexion(t_parametroEsperar parametros);
void esperar(t_parametroEsperar parametros);

void crear_metadata(char *nombre_archivo, int pos);
void modificar_metadata(char *nombre_archivo, char *parametro, int dato_modificar);
int info_archivo(char *nombre_archivo, char *parametro);
int division_redondeada(int numerador, int denominador);

#endif // !MAIN_IO