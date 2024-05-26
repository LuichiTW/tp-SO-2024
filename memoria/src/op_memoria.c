#include "op_memoria.h"

t_proceso* proceso_a;


void enviar_instruccion(int socket_cliente) {
    int pid = 1;
    int n_instruccion = 0;
    char* buffer;
    t_log logger = crear_memlogger();

    bool es_proceso_buscado(t_proceso * proceso) {
        return proceso->pid == pid;
    }

    // Recibir del buffer el pid y n_instruccion
    t_proceso* proceso = list_find(procesos_actuales, es_proceso_buscado);

    char** lista_instrucciones = proceso->instrucciones;

    if (n_instruccion >= proceso->cant_instrucciones) {
        // Se pasó del número
    }
    printf("%s\n", lista_instrucciones[n_instruccion]);
    
}


void crear_proceso(int socket_cliente) {
    int pid = 1;
    char* scriptname = "prueba.txt";
    int len = -1;
    t_proceso *proceso = malloc(sizeof(t_proceso));


    proceso->instrucciones = leer_script(scriptname);
    while (proceso->instrucciones[++len] != NULL){}

    proceso->pid = pid;
    proceso->cant_instrucciones = len;

    list_add(procesos_actuales, proceso);
}


char **leer_script(const char *filename) {
    
    // Concateno el nombre del archivo con la ruta de scripts
    char *path = string_from_format("%s/%s", config_memoria.path_instrucciones, filename);
    long filesize;
    char *content;
    char **lista;

    FILE *stream;
    stream = fopen(path, "r");

    // Obtengo el tamaño del archivo;
    fseek(stream, 0L, SEEK_END);
    filesize = ftell(stream);

    // Vuelvo al inicio del archivo
    fseek(stream, 0L, SEEK_SET);

    // Leo el contenido del script
    content = malloc((filesize + 1) * sizeof(char));
    fread(content, 1, filesize, stream);
    fclose(stream);

    // Agrego el \0 para terminar la cadena
    content[filesize] = '\0';

    // Guardo las líneas como array en lista, que sobreescribe a la original
    lista = string_split(content, "\n");

    free(content);

    return lista;
}