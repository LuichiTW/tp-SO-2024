#ifndef MAIN_CPU
#define MAIN_CPU

#include <stdlib.h>
#include <stdio.h>
#include <utils/conexiones/conexiones_servidor.h>
#include <utils/conexiones/conexiones_cliente.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/error.h>
#include <readline/readline.h>

#include "registros.h"
#include "decode.h"
#include "execute.h"
#include "listaInstrucciones.h"
#include "fetch.h"
#include "checkInterrupt.h"
typedef enum {
    GENERICAS, STDIN, STDOUT, DIALFS,
} tiposInterfacesIO;

//solicitar a memoria prox instruccion
enum OPERACIONES_MEMORIA {
    ENVIAR_INSTRUCCION
};

//operaciones de execute para el CPU
typedef enum {
    SET, MOV_IN, MOV_OUT, SUM, SUB, JNZ, RESIZE, COPY_STRING, WAIT, SIGNAL, IO_GEN_SLEEP, IO_STDIN_READ, IO_STDOUT_WRITE, IO_FS_CREATE, IO_FS_DELETE, IO_FS_TRUNCATE, IO_FS_WRITE, IO_FS_READ, EXIT,
} OP_EXECUTE_CPU;



#endif // !MAIN_CPU