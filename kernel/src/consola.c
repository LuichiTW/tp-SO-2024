#include "consola.h"

void iniciar_consola() {
    while (true) {
        char *comando_str = readline("> ");
        interpretar_comando(comando_str);
        free(comando_str);
    }
}

int traducir_comando(char *comando) {
    if (string_equals_ignore_case(comando, "EJECUTAR_SCRIPT")) return COM_EJECUTAR_SCRIPT;
    if (string_equals_ignore_case(comando, "INICIAR_PROCESO")) return COM_INICIAR_PROCESO;
    if (string_equals_ignore_case(comando, "FINALIZAR_PROCESO")) return COM_FINALIZAR_PROCESO;
    if (string_equals_ignore_case(comando, "DETENER_PLANIFICACION")) return COM_DETENER_PLANIFICACION;
    if (string_equals_ignore_case(comando, "INICIAR_PLANIFICACION")) return COM_INICIAR_PLANIFICACION;
    if (string_equals_ignore_case(comando, "MULTIPROGRAMACION")) return COM_MULTIPROGRAMACION;
    if (string_equals_ignore_case(comando, "PROCESO_ESTADO")) return COM_PROCESO_ESTADO;
    return -1;
}


void interpretar_comando(char *comando_str) {
    if (string_length(comando_str) == 0) return;

    char **comando_dividido = string_split(comando_str, " ");
    int comando = traducir_comando(comando_dividido[0]);

    if (comando == -1) {
        printf("ERROR: comando ingresado no válido.\n");
        return;
    }

    switch (comando) {
    case COM_EJECUTAR_SCRIPT:
        if (string_array_size(comando_dividido) < 2) {
            printf("ERROR: Parámetro faltante.\n");
            return;
        }
        c_ejecutar_script(comando_dividido[1]);
        break;
    case COM_INICIAR_PROCESO:
        if (string_array_size(comando_dividido) < 2) {
            printf("ERROR: Parámetro faltante.\n");
            return;
        }
        c_iniciar_proceso(comando_dividido[1]);
        break;
    case COM_FINALIZAR_PROCESO:
        if (string_array_size(comando_dividido) < 2) {
            printf("ERROR: Parámetro faltante.\n");
            return;
        }
        c_finalizar_proceso(atoi(comando_dividido[1]));
        break;
    case COM_DETENER_PLANIFICACION:
        c_detener_planificacion();
        break;
    case COM_INICIAR_PLANIFICACION:
        c_iniciar_planificacion();
        break;
    case COM_MULTIPROGRAMACION:
        if (string_array_size(comando_dividido) < 2) {
            printf("ERROR: Parámetro faltante.\n");
            return;
        }
        c_multiprogramacion(atoi(comando_dividido[1]));
        break;
    case COM_PROCESO_ESTADO:
        c_proceso_estado();
        break;
    }
    string_array_destroy(comando_dividido);
}


void c_ejecutar_script(char *path) {
    char **lista_comando = leer_script(path);
    int size_lista = string_array_size(lista_comando);
    for (int i = 0; i < size_lista; i++) {
        interpretar_comando(lista_comando[i]);
    }
    
}
void c_iniciar_proceso(char *path) {
    t_pcb *pcb = crear_proceso();

    t_paquete *paquete = crear_paquete();
    agregar_a_paquete(paquete, &(pcb->pid), sizeof(pcb->pid));
    agregar_a_paquete(paquete, path, string_length(path)+1);
    
    enviar_peticion(paquete, sockets.memoria, MEM_CREAR_PROCESO);
    eliminar_paquete(paquete);

    recibir_operacion(sockets.memoria);
    recibir_entero(sockets.memoria);

    queue_push(cola_new, pcb);
    t_log *logger = kernel_logger();
    log_info(logger, "Se crea el proceso %i en NEW", pcb->pid);
    log_destroy(logger);
    planificar();
}
void c_finalizar_proceso(int pid) {
    if (exec->pid == pid) {
        interrumpir("FINALIZAR_PROCESO");
    }
    else {
        t_pcb *pcb_fin = buscar_pcb_por_pid(pid);
        finalizar_proceso(pcb_fin);
    }
    planificar();
}
void c_detener_planificacion() {
    pthread_t t;
    pthread_create(&t, NULL, (void*)detener_planificacion_thread, NULL);
    pthread_detach(t);
}
void c_iniciar_planificacion() {
    sem_post(&sem_planificacion_general);
    sem_post(&sem_planificacion);
}
void c_multiprogramacion(int valor) {
    config_kernel.grado_multiprogramacion = valor;
    planificar();
}
void c_proceso_estado() {
    void imprimir_pid(void *vpcb) {
        t_pcb *pcb = (t_pcb*) vpcb;
        printf("| %i\n", pcb->pid);
    }
    printf("+---- NEW -----------------\n");
    list_iterate(cola_new->elements, imprimir_pid);
    printf("\n+---- READY ---------------\n");
    list_iterate(cola_ready->elements, imprimir_pid);
    printf("\n+---- READY PRIORIDAD -----\n");
    list_iterate(cola_ready_aux->elements, imprimir_pid);
    printf("\n+---- EXEC ----------------\n");
    if (exec != NULL) {
        printf("| %i\n", exec->pid);
    }
    printf("\n+---- BLOCKED -------------\n");
    print_bloqueados();
    printf("\n+---- EXIT ----------------\n");
    list_iterate(cola_exit->elements, imprimir_pid);
    printf("\n");
}


void print_bloqueados() {
    void print_si_bloqueado (void *vest) {
        t_estado_proceso *est = (*est) vest;
        if (est->motivo_bloqueo != BLOQUEO_NINGUNO) {
            printf("| %i\n", est->pid);
        }
    }
    list_iterate(lista_procesos, print_si_bloqueado);
}


void detener_planificacion_thread() {
    sem_wait(&sem_planificacion_general);
    sem_wait(&sem_planificacion);
}


char **leer_script(const char *path) {
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