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