#ifndef MAIN_IO
#define MAIN_IO

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <utils/conexiones/conexiones_cliente.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/error.h>
#include <pthread.h>
#include <readline/readline.h>
#include <utils/conexiones/conexiones_servidor.h>
#include <commons/string.h>
#include <commons/bitarray.h>



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
	char *puerto_memoria;
	char *ip_kernel;
	char *puerto_kernel;
} t_config_interfaz;

typedef struct t_metadata
{
	char *nombre;
	int bloque_inicial;
	struct t_metadata *siguiente;
} t_metadata;


typedef struct t_bloque
{
    size_t longitud;
    struct t_bloque *siguiente;
    char dato[];
}t_bloque;

enum tipo_interfaz
{
	GENERICA,
	STDIN,
	STDOUT,
	DIALFS
};



void manejarConexion(t_parametroEsperar parametros);
void esperar(t_parametroEsperar parametros);
void insertar_a_lista(t_metadata *nuevo);
void modificar_metadata(char *nombre_archivo, char *parametro, int dato_modificar);
char *terminacion_archivo(char *archivo, char *terminacion);
void actualizar_comienzo_lista(char *nombre_archivo,int posicion);

#endif // !MAIN_IO