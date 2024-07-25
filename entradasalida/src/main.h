#ifndef MAIN_IO
#define MAIN_IO

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
#include <commons/bitarray.h>

#include "interfaz.h"

void manejarConexion(t_parametroEsperar parametros);
void esperar(t_parametroEsperar parametros);

#endif // !MAIN_IO