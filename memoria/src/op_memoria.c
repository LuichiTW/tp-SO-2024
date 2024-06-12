#include "op_memoria.h"


void enviar_instruccion(int socket_cliente) {
    int pid = 1;
    int n_instruccion = 0;
    char* buffer;
    t_log *logger = alt_memlogger();

    bool es_proceso_buscado(t_proceso * proceso) {
        return proceso->pid == pid;
    }

    // TODO Recibir del buffer el pid y n_instruccion
    t_proceso* proceso = list_find(procesos_actuales, es_proceso_buscado);

    char** lista_instrucciones = proceso->instrucciones;

    if (n_instruccion >= proceso->cant_instrucciones) {
        // ! Se pasó del número
    }
    
}


void crear_proceso(int socket_cliente) {
    int pid = 1;
    char* scriptname = "prueba.txt";
    int len = -1;
    t_proceso *proceso = malloc(sizeof(t_proceso));


    // Crea la estructura de proceso, que tiene las líneas de código del script.
    proceso->instrucciones = leer_script(scriptname);
    while (proceso->instrucciones[++len] != NULL){}

    proceso->pid = pid;
    proceso->cant_instrucciones = len;

    list_add(procesos_actuales, proceso);

    // Crea la tabla de páginas para el proceso.
    t_tabla_paginas *tabla = crear_tabla_paginas(pid);
    list_add(tablas_paginas, tabla);
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



void resize_proceso(int pid, size_t nuevo_tam) {
    int cant_paginas_nueva = ceil(nuevo_tam / config_memoria.tam_pagina);
    int cant_paginas_actual;
    int cant_paginas_agregar;


    // Obtiene la tabla de páginas de este proceso.
    t_tabla_paginas *tabla_paginas;
    tabla_paginas = obtener_tabla_por_pid(pid);

    if (tabla_paginas == NULL) {
        // ! No se encontró la tabla
    }
    cant_paginas_actual = tabla_paginas->cant;

    // Calcula cuántas páginas debe agregar (o quitar, si da negativo).
    cant_paginas_agregar = cant_paginas_nueva - cant_paginas_actual;

    if (cant_paginas_agregar == 0) {
        // ! Retorna sin hacer nada
    }

    if (cant_paginas_agregar > 0) {
        if (cant_frames_libres() < cant_paginas_agregar) {
            // ! Out of memory
        }

        for (int i = 0; i < cant_paginas_agregar; i++) {
            int frame = prox_frame_libre();
            if (frame == -1) {
                // ! Out of memory (al cual no debería haber llegado pero igual)
            }
            
            // Reserva memoria para la nueva página y le asigna los valores.
            t_pagina *pag = malloc(sizeof(t_pagina));
            pag->frame = frame;
            pag->pagina = list_size(tabla_paginas->paginas);

            // Agrega la página a la tabla y setea el marco usado como ocupado.
            list_add(tabla_paginas, pag);
            tabla_paginas->cant++;
            bitarray_set_bit(frames_ocupados, frame);
        }
    }
    else {
        for (int i = cant_paginas_agregar; i < 0; i++) {
            // Quita la página de la tabla.
            t_pagina *pagina = list_remove(tabla_paginas->paginas, tabla_paginas->cant);

            // Libera el marco usado para la página.
            bitarray_clean_bit(frames_ocupados, pagina->frame);

            // Elimina la página.
            free(pagina);
            tabla_paginas->cant--;
        }
    }
}