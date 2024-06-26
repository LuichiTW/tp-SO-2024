#ifndef FILE_SYTEM
#define FILE_SYTEM

#include <stdlib.h>
#include <stdio.h>
#include <commons/error.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "interfaz.h"


void carga_archivos_fs(void);
void levantar_bloques(void);



#endifv // !FILE_SYTEM
