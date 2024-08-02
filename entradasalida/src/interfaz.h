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
#include <commons/bitarray.h>

//#include "main.h"
#include "fileSistem.h"


extern t_log *logger;
extern t_bitarray *bitmap;
extern t_bloque *bloques;
extern t_metadata *metadata;

extern t_config_interfaz *config_interfaz;
extern t_parametroEsperar parametros;
struct timespec tiempo;

// inicio programa
t_log *iniciar_logger_io(void);
t_config *iniciar_config(char *tipo_interfaz);
void manejo_config_interfaz(t_config *config);

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
void crear_metadata(char *nombre_archivo, int pos);
int info_archivo(char *nombre_archivo, char *parametro);

int division_redondeada(int numerador, int denominador);
int suma_array(int *array, int tamanio);
char* leer_subcadena(const char* cadena, size_t inicio, size_t fin);

#endif // !INTERFAZ_IO