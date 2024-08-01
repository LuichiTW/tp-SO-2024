#include "decode.h"

void *funDecode(char instruccionRecibida[]){
    char **a = string_split(instruccionRecibida, " ");
    struct s_instruccion* instruccion = malloc(sizeof(struct s_instruccion));
    instruccion->numInstruccion = decodificarInstruccion(a[0]);
    if (string_array_size(a) == 1) {
        instruccion->variables = NULL;
        return instruccion;
    }
    instruccion->variables = malloc(sizeof(struct nodo_instruccion));
    instruccion->variables->variable = a[1];
    instruccion->variables->siguiente=NULL;
    
    int i=2;
    while (a[i]!=NULL) {
        struct nodo_instruccion* temp = instruccion->variables;
        struct nodo_instruccion* VariableNueva = malloc(sizeof(struct nodo_instruccion));
        VariableNueva->variable = a[i];
        VariableNueva->siguiente = NULL;
        while (1) {
            if (temp->siguiente==NULL) {
                temp->siguiente=VariableNueva;
                break;
            }
            temp = temp->siguiente;
        }
        i++;
    }
    
    return instruccion;
}

int decodificarInstruccion(char instruccionADecodificar[])
{
    switch (string_length(instruccionADecodificar))
    {
    case 3:
        if (string_equals_ignore_case(instruccionADecodificar, "SET")) // SET
        {return 0;}
        else if (string_equals_ignore_case(instruccionADecodificar, "SUM")) // SUM
        {return 3;}
        else if (string_equals_ignore_case(instruccionADecodificar, "SUB")) // SUB
        {return 4;}
        else if (string_equals_ignore_case(instruccionADecodificar, "JNZ")) // JNZ
        {return 5;}
        break;

    case 4:
        if (string_equals_ignore_case(instruccionADecodificar, "WAIT")) // WAIT
        {return 8;}
        else if (string_equals_ignore_case(instruccionADecodificar, "EXIT")) // EXIT
        {return 18;}
        break;

    case 6:
        if (string_equals_ignore_case(instruccionADecodificar, "MOV_IN")) // MOV_IN
        {return 1;}
        else if (string_equals_ignore_case(instruccionADecodificar, "RESIZE")) // RESIZE
        {return 6;}
        else if (string_equals_ignore_case(instruccionADecodificar, "SIGNAL")) // SIGNAL
        {return 9;}
        break;

    case 7:
        if (string_equals_ignore_case(instruccionADecodificar, "MOV_OUT")) // MOV_OUT
        {return 2;}
        break;

    case 10:
        if (string_equals_ignore_case(instruccionADecodificar, "IO_FS_READ")) // IO_FS_READ
        {return 17;}
        break;

    case 11:
        if (string_equals_ignore_case(instruccionADecodificar, "COPY_STRING")) // COPY_STRING
        {return 7;}
        else if (string_equals_ignore_case(instruccionADecodificar, "IO_FS_WRITE")) // IO_FS_WRITE
        {return 16;}
        break;

    case 12:
        if (string_equals_ignore_case(instruccionADecodificar, "IO_GEN_SLEEP")) // IO_GEN_SLEEP
        {return 10;}
        else if (string_equals_ignore_case(instruccionADecodificar, "IO_FS_CREATE")) // IO_FS_CREATE
        {return 13;}
        else if (string_equals_ignore_case(instruccionADecodificar, "IO_FS_DELETE")) // IO_FS_DELETE
        {return 14;}
        break;

    case 13:
        if (string_equals_ignore_case(instruccionADecodificar, "IO_STDIN_READ")) // IO_STDIN_READ
        {return 11;}
        break;

    case 14:
        if (string_equals_ignore_case(instruccionADecodificar, "IO_FS_TRUNCATE")) // IO_FS_TRUNCATE
        {return 15;}
        break;

    case 15:
        if (string_equals_ignore_case(instruccionADecodificar, "IO_STDOUT_WRITE")) // IO_STDOUT_WRITE
        {return 12;}
        break;

    default:
        return -1;
        break;
    }
    return -1;
}

