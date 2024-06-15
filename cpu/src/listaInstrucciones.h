#ifndef LISTAINSTRUCCCIONES
#define LISTAINSTRUCCCIONES

#include <stdlib.h>
#include <stdio.h>
#include <utils/conexiones/conexiones_servidor.h>
#include <utils/conexiones/conexiones_cliente.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/error.h>
#include <readline/readline.h>
#include <commons/string.h>

struct nodo_instruccion
{
    char *variable;
    struct nodo_instruccion *siguiente;
};

struct s_instruccion {
    int numInstruccion;
    struct nodo_instruccion *variables;
};

char *obtener_n_poscion_lista(struct nodo_instruccion*, int);

void eliminar_Lista_Instruccion(struct s_instruccion*);



#endif // !LISTAINSTRUCCCIONES
