#include "execute.h"


void funExecute(struct s_instruccion *instruccion){
    char *varA, *varB, *varC, *varD, *varE;
    uint32_t numA=0;
    switch (instruccion->numInstruccion)
    {
    case 0: //SET
        varA = obtener_n_poscion_lista(instruccion->variables, 1);
        varB = obtener_n_poscion_lista(instruccion->variables, 2);
        sscanf(varB,"%d", &numA);
        fSET(decodificarRegistro(varA), numA);
        break;
    case 1: //MOV_IN
        varA = obtener_n_poscion_lista(instruccion->variables, 1);
        varB = obtener_n_poscion_lista(instruccion->variables, 2);
        fMOV_IN(decodificarRegistro(varA), decodificarRegistro(varB));
        break;
    case 2: //MOV_OUT
        varA = obtener_n_poscion_lista(instruccion->variables, 1);
        varB = obtener_n_poscion_lista(instruccion->variables, 2);
        fMOV_OUT(decodificarRegistro(varA), decodificarRegistro(varB));
        break;
    case 3: //SUM
        varA = obtener_n_poscion_lista(instruccion->variables, 1); 
        varB = obtener_n_poscion_lista(instruccion->variables, 2);
        fSUM(decodificarRegistro(varA), decodificarRegistro(varB));
        break;
    case 4: //SUB
        varA = obtener_n_poscion_lista(instruccion->variables, 1); 
        varB = obtener_n_poscion_lista(instruccion->variables, 2);
        fSUB(decodificarRegistro(varA), decodificarRegistro(varB));
        break;
    case 5: //JNZ
        varA = obtener_n_poscion_lista(instruccion->variables, 1);
        varB = obtener_n_poscion_lista(instruccion->variables, 2);
        sscanf(varB,"%d", &numA);
        fJNZ(decodificarRegistro(varA), numA);
        break;
    case 6: //RESIZE
        varA = obtener_n_poscion_lista(instruccion->variables, 1); 
        sscanf(varA,"%d", &numA);
        fRESIZE(numA);
        break;
    case 7: //COPY_STRING
        varA = obtener_n_poscion_lista(instruccion->variables, 1); 
        sscanf(varA,"%d", &numA);
        fCOPY_STRING(numA);
        break;
    case 8: //WAIT
        varA = obtener_n_poscion_lista(instruccion->variables, 1); 
        fWAIT(varA);
        break;
    case 9: //SIGNAL
        varA = obtener_n_poscion_lista(instruccion->variables, 1); 
        fSIGNAL(varA);
        break;
    case 10: //IO_GEN_SLEEP
        varA = obtener_n_poscion_lista(instruccion->variables, 1); 
        varB = obtener_n_poscion_lista(instruccion->variables, 2); 
        sscanf(varB,"%d", &numA);
        fIO_GEN_SLEEP(varA, numA);
        break;
    case 11: //IO_STDIN_READ
        varA = obtener_n_poscion_lista(instruccion->variables, 1); 
        varB = obtener_n_poscion_lista(instruccion->variables, 2); 
        varC = obtener_n_poscion_lista(instruccion->variables, 3); 
        fIO_STDIN_READ(varA,decodificarRegistro(varB),decodificarRegistro(varC));
        break;
    case 12: //IO_STDOUT_WRITE
        varA = obtener_n_poscion_lista(instruccion->variables, 1); 
        varB = obtener_n_poscion_lista(instruccion->variables, 2); 
        varC = obtener_n_poscion_lista(instruccion->variables, 3); 
        fIO_STDOUT_WRITE(varA,decodificarRegistro(varB),decodificarRegistro(varC));
        break;
    case 13: //IO_FS_CREATE
        varA = obtener_n_poscion_lista(instruccion->variables, 1); 
        varB = obtener_n_poscion_lista(instruccion->variables, 2); 
        fIO_FS_CREATE(varA, varB);
        break;
    case 14: //IO_FS_DELETE
        varA = obtener_n_poscion_lista(instruccion->variables, 1); 
        varB = obtener_n_poscion_lista(instruccion->variables, 2); 
        fIO_FS_DELETE(varA, varB);
        break;
    case 15: //IO_FS_TRUNCATE
        varA = obtener_n_poscion_lista(instruccion->variables, 1); 
        varB = obtener_n_poscion_lista(instruccion->variables, 2); 
        varC = obtener_n_poscion_lista(instruccion->variables, 3); 
        fIO_FS_TRUNCATE(varA,varB,decodificarRegistro(varC));
        break;
    case 16: //IO_FS_WRITE
        varA = obtener_n_poscion_lista(instruccion->variables, 1); 
        varB = obtener_n_poscion_lista(instruccion->variables, 2); 
        varC = obtener_n_poscion_lista(instruccion->variables, 3); 
        varD = obtener_n_poscion_lista(instruccion->variables, 4); 
        varE = obtener_n_poscion_lista(instruccion->variables, 5); 
        fIO_FS_WRITE(varA,varB,decodificarRegistro(varC),decodificarRegistro(varD),decodificarRegistro(varE));
        break;
    case 17: //IO_FS_READ
        varA = obtener_n_poscion_lista(instruccion->variables, 1); 
        varB = obtener_n_poscion_lista(instruccion->variables, 2); 
        varC = obtener_n_poscion_lista(instruccion->variables, 3); 
        varD = obtener_n_poscion_lista(instruccion->variables, 4); 
        varE = obtener_n_poscion_lista(instruccion->variables, 5); 
        fIO_FS_READ(varA,varB,decodificarRegistro(varC),decodificarRegistro(varD),decodificarRegistro(varE));
        break;
    case 18: //EXIT
        fEXIT();
        break;
    }
    
}

int decodificarRegistro(char variable[]){
    if (string_equals_ignore_case(variable, "AX")) {
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