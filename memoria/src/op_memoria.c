#include "op_memoria.h"


char *enviar_instruccion(int pid, int n_instruccion) {
    t_proceso *proceso;
    proceso = obtener_proceso_por_pid(pid);

    char** lista_instrucciones = proceso->instrucciones;

    if (n_instruccion >= proceso->cant_instrucciones) {
        // ! Se pasó del número
    }

    return lista_instrucciones[n_instruccion];
}


int crear_proceso(int pid, const char *scriptname) {
    int len = -1;
    t_proceso *proceso = malloc(sizeof(t_proceso));

    // Crea la estructura de proceso, que tiene las líneas de código del script.
    proceso->instrucciones = leer_script(scriptname);

    if (proceso->instrucciones == NULL) return 1; // ? No encontró el archivo

    while (proceso->instrucciones[++len] != NULL){}
    proceso->pid = pid;
    proceso->cant_instrucciones = len;

    list_add(procesos_actuales, proceso);

    // Crea la tabla de páginas para el proceso.
    t_tabla_paginas *tabla = crear_tabla_paginas(pid);
    list_add(tablas_paginas, tabla);

    return 0; // ? OK
}


void finalizar_proceso(int pid) {

    // Liberar espacio de memoria
    t_tabla_paginas *tabla;
    tabla = obtener_tabla_por_pid(pid);
    if (tabla != NULL) {
        resize_proceso(pid, 0);
        list_remove_element(tablas_paginas, tabla);
        free(tabla);
    }

    // Eliminar estructura de proceso
    t_proceso *proceso;
    proceso = obtener_proceso_por_pid(pid);
    if (proceso != NULL) {
        string_array_destroy(proceso->instrucciones);
        list_remove_element(procesos_actuales, proceso);
        free(proceso);
    }

}


char **leer_script(const char *filename) {
    // ? Podría usar las funciones de commons acá
    
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


int acceso_tabla_paginas(int pid, int pagina_buscada) {
    t_tabla_paginas *tabla;
    t_pagina *pagina;
    
    tabla = obtener_tabla_por_pid(pid);
    if (tabla == NULL) {
        // ! No encontró la tabla
    }

    pagina = list_get(tabla->paginas, pagina_buscada);
    if (pagina == NULL) {
        // ! Número de página no válido
    }
    
    t_log *logger = crear_memlogger();
    log_info(logger, "PID: %i - Pagina: %i - Marco: %i", pid, pagina_buscada, pagina->frame);
    log_destroy(logger);

    return pagina->frame;

}


int resize_proceso(int pid, int nuevo_tam) {
    int cant_paginas_nueva = iceildiv(nuevo_tam, config_memoria.tam_pagina);
    int cant_paginas_actual;
    int cant_paginas_agregar;


    // Obtiene la tabla de páginas de este proceso.
    t_tabla_paginas *tabla_paginas;
    tabla_paginas = obtener_tabla_por_pid(pid);

    if (tabla_paginas == NULL) {
        return 2; // ! No se encontró la tabla
    }
    cant_paginas_actual = tabla_paginas->cant;

    // Calcula cuántas páginas debe agregar (o quitar, si da negativo).
    cant_paginas_agregar = cant_paginas_nueva - cant_paginas_actual;

    if (cant_paginas_agregar == 0) {
        return 0;
    }

    if (cant_paginas_agregar > 0) {
        if (cant_frames_libres() < cant_paginas_agregar) {
            return 1;
        }

        for (int i = 0; i < cant_paginas_agregar; i++) {
            int frame = prox_frame_libre();
            if (frame == -1) {
                return 1;
            }
            
            // Reserva memoria para la nueva página y le asigna los valores.
            t_pagina *pag = malloc(sizeof(t_pagina));
            pag->frame = frame;
            //pag->pagina = list_size(tabla_paginas->paginas);

            // Agrega la página a la tabla y setea el marco usado como ocupado.
            list_add(tabla_paginas->paginas, pag);
            tabla_paginas->cant++;
            bitarray_set_bit(frames_ocupados, frame);
        }
        t_log *logger = crear_memlogger();
        log_info(logger, "PID: %i - Tamaño Actual: %i - Tamaño a Ampliar: %i", pid, cant_paginas_actual*config_memoria.tam_pagina, cant_paginas_nueva*config_memoria.tam_pagina);
        log_destroy(logger);
    }
    else {
        for (int i = cant_paginas_agregar; i < 0; i++) {
            // Quita la página de la tabla.
            t_pagina *pagina = list_remove(tabla_paginas->paginas, tabla_paginas->cant-1);

            // Libera el marco usado para la página.
            bitarray_clean_bit(frames_ocupados, pagina->frame);

            // Elimina la página.
            free(pagina);
            tabla_paginas->cant--;
        }
        t_log *logger = crear_memlogger();
        log_info(logger, "PID: %i - Tamaño Actual: %i - Tamaño a Reducir: %i", pid, cant_paginas_actual*config_memoria.tam_pagina, cant_paginas_nueva*config_memoria.tam_pagina);
        log_destroy(logger);
    }
    return 0;

}


void leer_memoria(int dir_fisica, size_t tamanio) {
    char *valor;
    valor = malloc(tamanio);
    memcpy(valor, (mem_usuario + dir_fisica), tamanio);

    printf("%s\n", valor);
}

void escribir_memoria(int dir_fisica, size_t tamanio, void *valor) {
    memcpy((mem_usuario + dir_fisica), valor, tamanio);
}


int iceildiv (int dividend, int divisor) {
    return dividend / divisor + (dividend % divisor != 0);
}