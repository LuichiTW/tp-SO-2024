#ifndef INTERFAZ_IO
#define INTERFAZ_IO

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

typedef struct
{
	int conexion_memoria;
	int conexion_kernel;
	int server_fd;
    int socket_cliente;
	t_log *logger;
} t_parametroEsperar;

enum tipo_interfaz {GENERICA, STDIN, STDOUT, DIALFS};

extern t_log *logger;

struct timespec tiempo;

//inicio programa
t_log *iniciar_logger_io(void);
t_config *iniciar_config(char *tipo_interfaz);
void cargar_config_interfaz(t_config config);

//instrucciones interfaz
int iO_GEN_SLEEP(t_parametroEsperar parametros);
int iO_STDIN_READ(t_parametroEsperar parametros);
int iO_STDOUT_WRITE(t_parametroEsperar parametros);
int iO_FS_CREATE(t_parametroEsperar parametros);
int iO_FS_DELETE(t_parametroEsperar parametros);
int iO_FS_TRUNCATE(t_parametroEsperar parametros);
int iO_FS_WRITE(t_parametroEsperar parametros);
int iO_FS_READ(t_parametroEsperar parametros);

//auxiliares
int leer_entero(char*buffer, int* desplazamiento);
int leer_64(char*buffer, int* desplazamiento);

char* leer_string(char* buffer, int* desplazamiento);
char** leer_array(char *buffer, int* desp);
int *leer_array_entero(char *buffer,int* desp);

void crear_metadata(char *nombre_archivo, int pos);
void modificar_metadata(char *nombre_archivo, char *parametro, int dato_modificar);
int info_archivo(char *nombre_archivo, char *parametro);
int division_redondeada(int numerador, int denominador);
char terminacion_archivo(char* archivo,char* terminacion);
int suma_array(int *array, int tamanio);


#endif // !INTERFAZ_IO