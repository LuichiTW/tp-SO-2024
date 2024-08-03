#ifndef ESCUCHA_DISPATCH_H
#define ESCUCHA_DISPATCH_H

#include "conexion.h"
#include "planificador.h"
#include "io.h"

void atender_dispatch();
void manejar_desalojo(char *motivo_raw);

#endif // ESCUCHA_DISPATCH_H