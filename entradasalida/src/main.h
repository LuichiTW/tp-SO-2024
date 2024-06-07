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
#include <utils/conexiones/conexiones_servidor.c>
#include "main.c"
#endif // !MAIN_IO

int leer_entero(char*buffer, int* desplazamiento)
{
	int ret;
	memcpy(&ret, buffer + (*desplazamiento), sizeof(int));
	(*desplazamiento)+=sizeof(int);
	return ret;
}

int leer_64(char*buffer, int* desplazamiento)
{
	int ret;
	memcpy(&ret, buffer + (*desplazamiento), sizeof(int));
	(*desplazamiento)+=sizeof(uint64_t);
	return ret;
}

char** leer_array(char*buffer, int* desp)
{
	int len = leer_entero(buffer, desp);
	char** arr = malloc((len+1)*sizeof(char*));

	for(int i = 0; i < len; i++)
	{
		arr[i] = leer_64(buffer, desp);
	}
	arr[len] = NULL;

	return arr;
}