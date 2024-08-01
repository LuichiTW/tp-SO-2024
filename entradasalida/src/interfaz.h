#ifndef INTERFAZ_IO
#define INTERFAZ_IO

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <dirent.h>
#include <readline/readline.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/error.h>
#include <commons/string.h>
#include <utils/conexiones/conexiones_servidor.h>
#include <utils/conexiones/conexiones_cliente.h>

#include "fileSistem.h"

typedef struct
{
	int conexion_memoria;
	int conexion_kernel;
	int server_fd;
	int socket_cliente;
	t_log *logger;
} t_parametroEsperar;

typedef struct
{
	int tiempo_unidad_trabajo;
	char *path_base_dialfs;
	int block_size;
	int block_count;
	int retraso_compactacion;
	char *ip_memoria;
	int puerto_memoria;
	char *ip_kernel;
	int puerto_kernel;
} t_config_interfaz;

typedef struct t_metadata
{
	char *nombre;
	char *bloque_inicial;
	struct t_metadata *siguiente;
} t_metadata;


enum tipo_interfaz
{
	GENERICA,
	STDIN,
	STDOUT,
	DIALFS
};

extern t_log *logger;
extern t_bitarray *bitmap;
extern t_bloque *bloques;

t_metadata *metadata;
t_config_interfaz config_interfaz;
t_parametroEsperar parametros;
struct timespec tiempo;

// inicio programa
t_log *iniciar_logger_io(void);
t_config *iniciar_config(char *tipo_interfaz);
void manejo_config_interfaz(t_config config);

// instrucciones interfaz
int iO_GEN_SLEEP(t_parametroEsperar parametros);
int iO_STDIN_READ(t_parametroEsperar parametros);
int iO_STDOUT_WRITE(t_parametroEsperar parametros);
int iO_FS_CREATE(t_parametroEsperar parametros);
int iO_FS_DELETE(t_parametroEsperar parametros);
int iO_FS_TRUNCATE(t_parametroEsperar parametros);
int iO_FS_WRITE(t_parametroEsperar parametros);
int iO_FS_READ(t_parametroEsperar parametros);

// auxiliares
int leer_entero(char *buffer, int *desplazamiento);
char *leer_string(char *buffer, int *desplazamiento);
int *leer_array_entero(char *buffer, int *desp);

// funciones metadata //? podrian ir en otro archivo
t_metadata *cargar_metadata(t_config_interfaz *config_dialfs);
t_metadata *crear_metadata(char *nombre_archivo, int pos);
t_metadata *agregar_a_lista(t_metadata *cabeza, t_metadata *nuevo);
void modificar_metadata(char *nombre_archivo, char *parametro, int dato_modificar);
int info_archivo(char *nombre_archivo, char *parametro);

int division_redondeada(int numerador, int denominador);
char terminacion_archivo(char *archivo, char *terminacion);
int suma_array(int *array, int tamanio);
void limpiar_archivo_bitmap(char *archivo);
void agregar_archivo_bitmap(char *archivo,int tamanio);

#endif // !INTERFAZ_IO