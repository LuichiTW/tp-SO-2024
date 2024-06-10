#include "main.h"

int main() {
    t_log *logger = log_create("io.log", "io", true, LOG_LEVEL_INFO);
    log_info(logger, "Iniciando I/O...");
    t_config *config = config_create("./io.config");
    
    
    if (config == NULL) {
        log_error(logger, "No se leyo el archivo de configuracion");
        exit(EXIT_FAILURE);
    }
    char *tipo_interfaz = config_get_string_value(config,"TIPO_INTERFAZ");
    int tiempo_unidad_trabajo = config_get_int_value(config,"TIEMPO_UNIDAD_TRABAJO");
    char *path_base_dialfs = config_get_string_value(config,"PATH_BASE_DIALFS");
    int block_size = config_get_int_value(config,"BLOCK_SIZE");
    int block_count = config_get_int_value(config,"BLOCK_COUNT");
    int retraso_compactacion = config_get_int_value(config,"RETRASO_COMPACTACION");
    //memoria
    char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    //kernel
    char *ip_kernel = config_get_string_value(config, "IP_KERNEL");
    char *puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");


    tiempo.tv_sec = tiempo_unidad_trabajo;
    
    t_parametroEsperar parametros;

    
    //cliente a memoria
    parametros.conexion_memoria = crear_conexion(ip_memoria, puerto_memoria, "Hola soy el IO");
   

    //cliente a kernel
    parametros.conexion_kernel = crear_conexion(ip_kernel, puerto_kernel, "Hola soy el IO");
   
    //servidor

    parametros.server_fd = iniciar_servidor(puerto_kernel);
    parametros.logger = logger;

    pthread_t kernel_t;
    pthread_create(&kernel_t,NULL,(void*)esperar,(void*)&parametros);
    pthread_detach(kernel_t);


    return 0;
    ///////////////////////////////////////////////////////////////////
}

void manejarConexion(t_parametroEsperar parametros){
    int tipoInterfaz = recibir_operacion(parametros.socket_cliente);
    int resultado;

    switch(tipoInterfaz){
        case 0: //IO_GEN_SLEEP
        resultado = iO_GEN_SLEEP(parametros);
        break;
        case 1: //IO_STDIN_READ
        resultado =iO_STDIN_READ(parametros);
        break;
        case 2: //IO_STDOUT_WRITE
        resultado = iO_STDOUT_WRITE(parametros);
        break;
        default:
            log_error(parametros.logger, "Tipo de interfaz desconocida: %d", tipoInterfaz);
    }
    if(resultado == 0){
        enviar_mensaje("OK",parametros.conexion_kernel);
    }else{
        enviar_mensaje("ERROR",parametros.conexion_kernel);
    }
}


void esperar(t_parametroEsperar parametros){
   
    while(1){
        parametros.socket_cliente = esperar_cliente(parametros.server_fd,parametros.logger);     
        
        pthread_t t;
        pthread_create(&t,NULL,(void*)manejarConexion,(void*)&parametros);
        pthread_detach(t);
    }
}


int iO_GEN_SLEEP(t_parametroEsperar parametros){
    int size;
    char * buffer;
    int desp = 0;

    buffer = recibir_buffer(&size,parametros.socket_cliente);

    int uTrabajo = leer_entero(buffer,&desp);
    for (int i = 0; i < uTrabajo; i++) {
        nanosleep(&tiempo, NULL);
    }
    if(uTrabajo == NULL){
        return 1;
    }
    free(uTrabajo);
    free(buffer);
    return 0;
}

int iO_STDIN_READ(t_parametroEsperar parametros){
    int size;
    char * buffer;
    int desp = 0;
    char* texto;

    buffer = recibir_buffer(&size,parametros.socket_cliente);
	char direcciones[sizeof(leer_array(buffer,&desp))];
	memcpy(direcciones,leer_array(buffer,&desp),sizeof(direcciones));

    if(direcciones == NULL){
        return 1;
    }

    texto = readline("> ");
	t_paquete *paquete = crear_paquete();
	agregar_a_paquete (paquete, texto , strlen(texto) + 1);
	agregar_a_paquete (paquete, direcciones, sizeof(direcciones)+1);

	enviar_paquete(paquete ,parametros.conexion_memoria);
    
    free(texto);
    free(buffer);
    free(direcciones);
    return 0;
}

int iO_STDOUT_WRITE(t_parametroEsperar parametros){
    int size;
    char * buffer;
    int desp = 0;
    char* texto;

    buffer = recibir_buffer(&size,parametros.socket_cliente);     
	char *direcciones[sizeof(leer_array(buffer,&desp))];     
	memcpy(direcciones,leer_array(buffer,&desp),sizeof(direcciones)); 
    if(direcciones == NULL){
        return 1;
    }
    t_paquete *paquete = crear_paquete();
    agregar_a_paquete (paquete, direcciones, sizeof(direcciones)+1);
    enviar_paquete(paquete ,parametros.conexion_memoria);

    int socketCliente = esperar_cliente(parametros.conexion_memoria,parametros.logger);
    buffer = recibir_buffer(&size,socketCliente);
    char *direccionesT[sizeof(leer_array(buffer,&desp))]; 
    memcpy(direccionesT,leer_array(buffer,&desp),sizeof(direccionesT));
    if(direccionesT == NULL){
        return 1;
    }
    size_t num_direcciones = sizeof(direccionesT) / sizeof(direccionesT[0]);

    for (size_t i = 0; i < num_direcciones; ++i){
        char *direccionT = (char*)direccionesT[i];
        strncpy(texto, direccionT, &size);
        printf("Texto leido: %c\n", texto);
    }

    free(texto);
    free(buffer);
    free(direcciones);
    free(direccionesT);
    return 0;
}

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