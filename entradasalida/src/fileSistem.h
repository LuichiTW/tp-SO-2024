#ifndef FILE_SYTEM
#define FILE_SYTEM

#include <stdlib.h>
#include <stdio.h>
#include <commons/error.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/bitarray.h>
#include <commons/string.h>
#include <dirent.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "main.h"
#include "interfaz.h"

extern t_log *logger;
extern t_config_interfaz *config_interfaz;

extern t_bitarray *bitmap;
extern t_bloque *bloques;
extern t_metadata *metadata;


char *buscar_metadata(int posicion);
void comprobar_filesystem(t_config_interfaz *config_dialfs);
int archivo_esta_vacio(char *nombre_archivo);

void compactacion(t_bloque *bloques, t_bitarray *bitmap);
void compactacion_metadata(void);
void insertar_archivo_bloques(t_bloque *archivo);
t_bloque* extraer_parte_lista(t_bloque *cabeza, int indice_inicial, int cantidad);
int crear_archivo_bloques(t_bloque *cabeza);
void eliminar_archivo_bloques(t_bloque *cabeza, int bloque_inicial, int tamanio);
t_bloque *compactar_bloque(t_bloque *bloques);

t_bloque *levantar_bloques(t_config_interfaz *config_dialfs);
t_bitarray *crear_bitmap(t_config_interfaz *config_dialfs);
void actualizar_bitmap(t_bitarray *bitmap, t_bloque *bloques);
void guardar_bitmap(t_bitarray *bitmap, t_config_interfaz *config_dialfs);
t_bitarray *cargar_bitmap(t_config_interfaz *config_dialfs);
void imprimir_bitmap(t_bitarray *bitmap);
t_bloque *crear_bloque(size_t tamano_dato);
t_bloque *insertarAlFinal(t_bloque *cabeza, t_config *config_dialfs, char *dato);
void guardarListaEnArchivo(t_bloque *cabeza, char *nombreArchivo);
t_bloque *leerListaDesdeArchivo(char *nombreArchivo);
void liberarLista(t_bloque *cabeza);
int crear_archivo_bloques(t_bloque *cabeza);
void eliminar_archivo_bloques(t_bloque *cabeza, int bloque_inicial, int tamanio);
t_metadata *cargar_metadata(t_config_interfaz *config_dialfs);



#endif // !FILE_SYTEM