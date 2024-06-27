#ifndef FILE_SYTEM
#define FILE_SYTEM

#include <stdlib.h>
#include <stdio.h>
#include <commons/error.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/bitarray.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "interfaz.h"

typedef struct t_bloque {
    void *dato;
    struct t_bloque *siguiente;
} t_bloque;


void carga_archivos_fs(void);
t_bloque levantar_bloques(void);



#endif // !FILE_SYTEM
