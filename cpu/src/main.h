#ifndef MAIN_CPU
#define MAIN_CPU

#include <stdlib.h>
#include <stdio.h>
#include <utils/conexiones/conexiones_servidor.h>
#include <utils/conexiones/conexiones_cliente.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/error.h>
#include <commons/string.h>
#include <readline/readline.h>
#include <pthread.h>
#include <semaphore.h>

#include "registros.h"
#include "cicloDeEjecucion/decode.h"
#include "cicloDeEjecucion/execute.h"
#include "listaInstrucciones.h"
#include "cicloDeEjecucion/fetch.h"
//#include "cicloDeEjecucion/checkInterrupt.h"
#include "config.h"
#include "conexiones.h"
#include "pcb.h"

#endif // !MAIN_CPU