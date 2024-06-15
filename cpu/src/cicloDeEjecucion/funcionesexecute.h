#ifndef FUNEXECUTE
#define FUNEXECUTE

#include <stdlib.h>
#include <stdio.h>
#include <utils/conexiones/conexiones_servidor.h>
#include <utils/conexiones/conexiones_cliente.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/error.h>
#include <readline/readline.h>

#include "registros.h"
#include "config.h"

// #include "main.h"
extern t_log *loggerPrincipal;

//suma dos registros y coloca el resultado en el primer registro
void fSUM(enum lista_registros_CPU, enum lista_registros_CPU);

//resta dos registros y coloca el resultado en el primer registro
void fSUB(enum lista_registros_CPU, enum lista_registros_CPU);

//Establece el valor pasado en el registro indicado
void fSET(enum lista_registros_CPU, int);

void fJNZ(enum lista_registros_CPU, uint32_t);

void fMOV_IN(enum lista_registros_CPU, enum lista_registros_CPU);

void fMOV_OUT(enum lista_registros_CPU, enum lista_registros_CPU);

void fRESIZE(int);

void fCOPY_STRING(int ramanho);

void fWAIT(char recurso[]);

void fSIGNAL(char recurso[]);

void fIO_GEN_SLEEP(char interface[], int unidadesDeTrabajo);

void fIO_STDIN_READ(char interface[], enum lista_registros_CPU Dirrecion, enum lista_registros_CPU Tamanho);

void fIO_STDOUT_WRITE(char interface[], enum lista_registros_CPU Dirrecion, enum lista_registros_CPU Tamanho);

void fIO_FS_CREATE(char interface[], char NombreArchivo[]);

void fIO_FS_DELETE(char interface[], char NombreArchivo[]);

void fIO_FS_TRUNCATE(char interface[], char NombreArchivo[], enum lista_registros_CPU Tamanho);

void fIO_FS_WRITE(char interface[], char NombreArchivo[], enum lista_registros_CPU Direccion, enum lista_registros_CPU Tamanho, enum lista_registros_CPU PunteroArchivo);

void fIO_FS_READ(char interface[], char NombreArchivo[], enum lista_registros_CPU Direccion, enum lista_registros_CPU Tamanho, enum lista_registros_CPU PunteroArchivo);

void fEXIT();

#endif // FUNEXECUTE