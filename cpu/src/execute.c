#include "execute.h"


void funExecute(struct s_instruccion *instruccion){
    char *varA, *varB/* , *varC, *varD, *varE */;
    uint32_t numA=0/* , numB=0, numC=0, numD=0, numE=0 */;
    switch (instruccion->numInstruccion)
    {
    case 0: //SET
        varA = obtener_n_poscion_lista(instruccion->variables, 2);
        sscanf(varA,"%d", &numA);
        varB = obtener_n_poscion_lista(instruccion->variables, 1);
        fSET((int)obtenerRegistro((char)varB), numA);
        break;
    case 1: //MOV_IN
        
        break;
    case 2: //MOV_OUT
        
        break;
    case 3: //SUM
        varA = obtener_n_poscion_lista(instruccion->variables, 1); 
        varB = obtener_n_poscion_lista(instruccion->variables, 2);
        fSUM((int)obtenerRegistro((char)varA), (int)obtenerRegistro((char)varB));
        break;
    case 4: //SUB
        varA = obtener_n_poscion_lista(instruccion->variables, 1); 
        varB = obtener_n_poscion_lista(instruccion->variables, 2);
        fSUB((int)obtenerRegistro((char)varA), (int)obtenerRegistro((char)varB));
        break;
    case 5: //JNZ
        varA = obtener_n_poscion_lista(instruccion->variables, 2);
        sscanf(varA,"%d", &numA);
        varB = obtener_n_poscion_lista(instruccion->variables, 1);
        fJNZ((int)obtenerRegistro((char)varB), numA);
        break;
    case 6: //RESIZE
        
        break;
    case 7: //COPY_STRING
        
        break;
    case 8: //WAIT
        
        break;
    case 9: //SIGNAL
        
        break;
    case 10: //IO_GEN_SLEEP
        
        break;
    case 11: //IO_STDIN_READ
        
        break;
    case 12: //IO_STDOUT_WRITE
        
        break;
    case 13: //IO_FS_CREATE
        
        break;
    case 14: //IO_FS_DELETE
        
        break;
    case 15: //IO_FS_TRUNCATE
        
        break;
    case 16: //IO_FS_WRITE
        
        break;
    case 17: //IO_FS_READ
        
        break;
    case 18: //EXIT
        
        break;
    }
    
}

int decodificarRegistro(char variable[]){
    if (string_equals_ignore_case(variable, "")) {
        
    } else if (string_equals_ignore_case(variable, "AX")) {
        return 0;
    } else if (string_equals_ignore_case(variable, "BX")) {
        return 1;
    } else if (string_equals_ignore_case(variable, "CX")) {
        return 2;
    } else if (string_equals_ignore_case(variable, "DX")) {
        return 3;
    } else if (string_equals_ignore_case(variable, "EAX")) {
        return 4;
    } else if (string_equals_ignore_case(variable, "EBX")) {
        return 5;
    } else if (string_equals_ignore_case(variable, "ECX")) {
        return 6;
    } else if (string_equals_ignore_case(variable, "EDX")) {
        return 7;
    } else if (string_equals_ignore_case(variable, "PC")) {
        return 8;
    } else if (string_equals_ignore_case(variable, "SI")) {
        return 9;
    } else if (string_equals_ignore_case(variable, "DI")) {
        return 10;
    }
    
    return -1;
}