#ifndef FILE_SYTEM
#define FILE_SYTEM

#include <stdlib.h>
#include <stdio.h>
#include <commons/error.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/bitarray.h>
#include <commons/string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "interfaz.h"

typedef struct t_bloque
{
    char dato[1024];
    struct t_bloque *siguiente;
} t_bloque;


void compactacion(t_bloque *bloques, t_bitarray *bitmap);
t_bloque *compactar_bloque(t_bloque *bloques);
void carga_archivos_fs(void);
t_bloque *levantar_bloques(void);
void crear_bitmap(void);
t_bloque *insertarAlFinal(t_bloque *cabeza, char *dato);
void guardarListaEnArchivo(t_bloque *cabeza, char *nombreArchivo);
t_bloque *leerListaDesdeArchivo(char *nombreArchivo);
void liberarLista(t_bloque *cabeza);




#endif // !FILE_SYTEM
