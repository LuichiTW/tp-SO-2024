#ifndef CONEXIONES_CLIENTE
#define CONEXIONES_CLIENTE
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>

typedef enum
{
	MENSAJE,
	PAQUETE
}op_code;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

/**
 * @fn crear_conexion
 * @brief 
 * @param ip direccion donde nos vamos a conectar
 * @param puerto puerto en el cual nos vamos a conectar
 * @param identificador nombre de quien se esta identificando
 * @return retrona el socket del servidor
*/
int crear_conexion(char* ip, char* puerto, char* identificador);
/**
 * @fn enviar_mensaje
 * @brief 
 * @param mensaje
 * @param socket_cliente
 * @return 
*/
void enviar_mensaje(char* mensaje, int socket_cliente);
/**
 * @fn crear_paquete
 * @brief 
 * @return 
*/
t_paquete* crear_paquete(void);
/**
 * @fn agregar_a_paquete
 * @brief 
 * @param paquete
 * @param valor
 * @param tamanio
 * @return 
*/
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
/**
 * @fn enviar_paquete
 * @brief 
 * @param paquete
 * @param socket_cliente
 * @return 
*/
void enviar_paquete(t_paquete* paquete, int socket_cliente);
/**
 * @fn liberar_conexion
 * @brief 
 * @param socket_cliente
 * @return 
*/
void liberar_conexion(int socket_cliente);
/**
 * @fn eliminar_paquete
 * @brief 
 * @param 
 * @return 
*/
void eliminar_paquete(t_paquete* paquete);

#endif // !CONEXIONES_CLIENTE
 * @param mensaje
 * @param socket_cliente
 * @return 
*/
void enviar_mensaje(char* mensaje, int socket_cliente);
/**
 * @fn crear_paquete
 * @brief 
 * @return 
*/
t_paquete* crear_paquete(void);
/**
 * @fn agregar_a_paquete
 * @brief 
 * @param paquete
 * @param valor
 * @param tamanio
 * @return 
*/
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
/**
 * @fn enviar_paquete
 * @brief 
 * @param paquete
 * @param socket_cliente
 * @return 
*/
void enviar_paquete(t_paquete* paquete, int socket_cliente);
/**
 * @fn liberar_conexion
 * @brief 
 * @param socket_cliente
 * @return 
*/
void liberar_conexion(int socket_cliente);
/**
 * @fn eliminar_paquete
 * @brief 
 * @param 
 * @return 
*/
void eliminar_paquete(t_paquete* paquete);

#endif // !CONEXIONES_CLIENTE