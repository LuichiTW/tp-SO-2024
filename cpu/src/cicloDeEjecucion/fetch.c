#include "fetch.h"

char *funFetch(int socketMemoria){
    //!PRUEBA DE FUNCIONAMIENTO MANUAL
    char *instruccion = readline("> ");
    //!-------------------------------

    uint32_t numero = regcpu.PC;
    // Serializar el tipo de operación, tamaño del dato y el número
    int tipo_operacion = PROXIMAINSTRUCCION;
    int tamano_dato = sizeof(uint32_t);
    
    // Crear un buffer para almacenar los datos serializados
    int buffer_size = sizeof(tipo_operacion) + sizeof(tamano_dato) + sizeof(numero);
    char *buffer = (char *)malloc(buffer_size);
    
    // Copiar los datos serializados en el buffer
    int offset = 0;
    memcpy(buffer + offset, &tipo_operacion, sizeof(tipo_operacion));
    offset += sizeof(tipo_operacion);
    memcpy(buffer + offset, &tamano_dato, sizeof(tamano_dato));
    offset += sizeof(tamano_dato);
    memcpy(buffer + offset, &numero, sizeof(numero));

    //!FALTA ENVIAR EL BUFFER A MEMORIA Y ESPERAR A QUE LLEGUE LA RESPUESTA CON EL CHAR PARA HACER EL RETURN

    regcpu.PC++;
    return instruccion;
}

