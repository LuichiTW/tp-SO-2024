#include "listaInstrucciones.h"

char *obtener_n_poscion_lista(struct nodo_instruccion *instruccion, int n){
    int i = 1;
    while (1) {
        if (i == n) {
        return instruccion->variable;
        }
        instruccion = instruccion->siguiente;
        i++;
    }
}

void eliminar_Lista_Instruccion(struct s_instruccion *instruccion){
    if (instruccion != NULL) {
        struct nodo_instruccion *temp;
        while (instruccion->variables != NULL) {
            temp = instruccion->variables;
            instruccion->variables = instruccion->variables->siguiente;
            if (temp->variable != NULL) {
                free(temp->variable);
            }
            free(temp);
        }
        free(instruccion);
    }
    
}