#include "utils/conexiones/conexiones_servidor.h"

int iniciar_servidor(char* puerto){
    int socket_servidor;
    struct addrinfo hints, *serverinfo;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo(NULL, puerto, &hints, &serverinfo);
    socket_servidor = socket(serverinfo->ai_family,serverinfo->ai_socktype,serverinfo->ai_protocol);
    if(socket_servidor == -1){
        freeaddrinfo(serverinfo);
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }
    bind(socket_servidor, serverinfo->ai_addr, serverinfo->ai_addrlen);
    freeaddrinfo(serverinfo);
    listen(socket_servidor, SOMAXCONN);
    return socket_servidor;
}
int esperar_cliente(int socket_servidor, t_log *logger){
    int socket_cliente = accept(socket_servidor, NULL, NULL);
    if(socket_cliente == -1){
        return -1;
    }
    log_info(logger, "Se conecto un Cliente.");
    recibir_operacion(socket_cliente);
    recibir_mensaje(socket_cliente, logger);
    return socket_cliente;
}
int recibir_operacion(int socket_cliente){
    int codigo_operacion;
    if (recv(socket_cliente, &codigo_operacion, sizeof(int), MSG_WAITALL)>0)
    {
        return codigo_operacion;
    } else {
        close(socket_cliente);
        return -1;
    }
    
}
void* recibir_buffer(int* size, int socket_cliente){
    void* buffer;
    recv (socket_cliente, size, sizeof(int), MSG_WAITALL);
    buffer = malloc(*size);
    recv(socket_cliente, buffer, *size, MSG_WAITALL);
    return buffer;
}
void recibir_mensaje(int socket_cliente, t_log *logger)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}
t_list* recibir_paquete(int socket_cliente)
{
	int size, desplazamiento = 0, tamanho;
	t_list* valores = list_create();
	void * buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size) {
		memcpy(&tamanho, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanho);
		memcpy(valor, buffer+desplazamiento, tamanho);
		desplazamiento+=tamanho;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}

int recibir_entero(int socket_cliente) {
    char *rta = recibir_msg(socket_cliente);

    int valor = atoi(rta);
    free(rta);
    return valor;
}

char *recibir_msg(int socket_cliente) {
    int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
    return buffer;
}


int esperar_cliente2(int socket_servidor, t_log *logger){
    int socket_cliente = accept(socket_servidor, NULL, NULL);
    if(socket_cliente == -1){
        return -1;
    }
    log_info(logger, "Se conecto un Cliente.");
    return socket_cliente;
}