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
	char * tipo_interfaz;
	int tiempo_unidad_trabajo;
    char *ip_kernel;
    char *puerto_kernel;
} t_config_generica;

typedef struct
{
	char * tipo_interfaz;
    char *ip_memoria;
    char *puerto_memoria;
    char *ip_kernel;
    char *puerto_kernel;
} t_config_stdin;

typedef struct
{
	char * tipo_interfaz;
    char *ip_memoria;
    char *puerto_memoria;
    char *ip_kernel;
    char *puerto_kernel;
} t_config_stdout;

typedef struct
{
	char * tipo_interfaz;
	int tiempo_unidad_trabajo;
	char *path_base_dialfs;
	int block_size;
    int block_count;
    int retraso_compactacion;
    char *ip_memoria;
    char *puerto_memoria;
    char *ip_kernel;
    char *puerto_kernel;
} t_config_dialfs;

typedef struct t_interfaz t_interfaz;

struct t_interfaz
{
    char * nombre;
    char * path_interfaz;
    t_interfaz * siguiente;
};

enum tipo_interfaz {GENERICA, STDIN, STDOUT, DIALFS};

typedef struct
{
  char *path_config_l;
  enum tipo_interfaz id;
}t_interfaz_disponibles;


t_log *iniciar_logger_io(void);
t_config *iniciar_config(char *tipo_interfaz);
void *iniciar_config_io(t_interfaz *interfaces);
t_interfaz *crear_interfaces(void);
t_interfaz *agregar_interfaz( t_interfaz *interfaces, t_interfaz *nueva_interfaz);
void recorrer_lista_nombres_tipos(t_interfaz *interfaces);
bool es_path_config(char *path_config);
int id_path_config(char *path_config);

#endif // !INTERFAZ_IO