#ifndef MAIN_IO
#define MAIN_IO

#include <stdlib.h>
#include <stdio.h>
#include <utils/conexiones/conexiones_cliente.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/error.h>
#include <pthread.h>
#include <readline/readline.h>
#include <utils/conexiones/conexiones_servidor.c>
#endif // !MAIN_IO

struct timespec tiempo;

typedef struct
{
	int conexion_memoria;
	int conexion_kernel;
	int server_fd;
    int socket_cliente;
	t_log *logger;

} t_parametroEsperar;

void iO_GEN_SLEEP(t_parametroEsperar parametros);
void iO_STDIN_READ(t_parametroEsperar parametros);
void iO_STDOUT_WRITE(t_parametroEsperar parametros);


int leer_entero(char*buffer, int* desplazamiento)
{
	int ret;
	memcpy(&ret, buffer + (*desplazamiento), sizeof(int));
	(*desplazamiento)+=sizeof(int);
	return ret;
}

int leer_64(char*buffer, int* desplazamiento)
{
	int ret;
	memcpy(&ret, buffer + (*desplazamiento), sizeof(int));
	(*desplazamiento)+=sizeof(uint64_t);
	return ret;
}


char* leer_string(char* buffer, int* desplazamiento) 
{          
	int tamanio = leer_entero(buffer, desplazamiento);          
	char* valor = malloc(tamanio);     
	memcpy(valor, buffer+(*desplazamiento), tamanio);     
	(*desplazamiento)+=tamanio;      
	return valor; 
}  
char** leer_array(char *buffer, int* desp) 
{     
		int len = leer_entero(buffer, desp);     
		char** arr = malloc((len+1) *sizeof(char*));      
		for(int i = 0; i < len; i++)     
		{         
			arr[i] = leer_string(buffer, desp);     
		}     
		arr[len] = NULL;      
		return arr; 

}


void manejarConexion(t_parametroEsperar parametros){
    int tipoInterfaz = recibir_operacion(parametros.socket_cliente);
    

    
    switch(tipoInterfaz){
        case 0: //IO_GEN_SLEEP
        iO_GEN_SLEEP(parametros);
        break;
        case 1: //IO_STDIN_READ
        iO_STDIN_READ(parametros);
        break;
        case 2: //IO_STDOUT_WRITE
        iO_STDOUT_WRITE(parametros);
        break;
    }
}

void esperar(t_parametroEsperar parametros){
   
    while(1){
        //parametros.socket_cliente = esperar_cliente(parametros.server_fd,parametros.logger);     
        
        pthread_t t;
        pthread_create(&t,NULL,(void*)manejarConexion,(void*)&parametros);
        pthread_detach(t);
    }
}


void iO_GEN_SLEEP(t_parametroEsperar parametros){
    int size;
    char * buffer;
    int desp = 0;

    buffer = recibir_buffer(&size,parametros.socket_cliente);

    int uTrabajo = leer_entero(buffer,&desp);
    for (int i = 0; i < uTrabajo; i++) {
        nanosleep(&tiempo, NULL);
    }
    free(buffer);
}

void iO_STDIN_READ(t_parametroEsperar parametros){
    int size;
    char * buffer;
    int desp = 0;
    char* texto;

    buffer = recibir_buffer(&size,parametros.socket_cliente);
	char direcciones[sizeof(leer_array(buffer,&desp))];
	memcpy(direcciones,leer_array(buffer,&desp),sizeof(direcciones));

    texto = readline("> ");
	t_paquete *paquete = crear_paquete();
	agregar_a_paquete (paquete, texto , sizeof(texto));
	agregar_a_paquete (paquete, direcciones, sizeof(direcciones));

	enviar_paquete(paquete ,parametros.conexion_memoria);
    
    free(texto);
    free(buffer);
    free(direcciones);
}

void iO_STDOUT_WRITE(t_parametroEsperar parametros){
    int size;
    char * buffer;
    int desp = 0;
    char* texto;

    buffer = recibir_buffer(&size,parametros.socket_cliente);     
	char direcciones[sizeof(leer_array(buffer,&desp))];     
	memcpy(direcciones,leer_array(buffer,&desp),sizeof(direcciones));     
	


    free(texto);
    free(buffer);
    free(direcciones);
}