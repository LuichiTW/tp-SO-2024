#include "mmu.h"

t_dir_logica separar_dir_logica(int dir_logica) {
    t_dir_logica dir_separada;

    dir_separada.pagina = (int) floor((double)(dir_logica / tam_pagina));
    dir_separada.desplazamiento = dir_logica - dir_separada.pagina * tam_pagina;

    return dir_separada;
}



t_lista_dir_fisicas obtener_direcciones_fisicas(int dir_logica_i, int tam_dato) {
    t_dir_logica dir_logica = separar_dir_logica(dir_logica_i);

    int cant_paginas = fraccionar_direccion(dir_logica, tam_dato);
    
    int bytes_por_pag[cant_paginas];
    int dir_fisicas[cant_paginas];

    // Cuántos bytes del dato van en cada página

    {
        int res = tam_pagina - dir_logica.desplazamiento;
        if (tam_dato < res) {
            bytes_por_pag[0] = tam_dato;
        }
        else {
            bytes_por_pag[0] = res;
        }
        tam_dato -= res;
    }
    for (int i = 1; tam_dato > 0; i++)
    {
        int res = tam_pagina;
        if (tam_dato < res) {
            bytes_por_pag[i] = tam_dato;
        }
        else {
            bytes_por_pag[i] = res;
        }
        tam_dato -= res;
    }
    
    

    /* bytes_por_pag[0] = tam_pagina - dir_logica.desplazamiento;
    for (int i = 1; i < cant_paginas - 1; i++) {
        bytes_por_pag[i] = tam_pagina;
    }
    // Si cant_paginas = 1, cambiaría bytes_por_pag[0], que es el primero (y no debe pasar)
    if (cant_paginas > 1) {
        bytes_por_pag[cant_paginas-1] = dir_logica.desplazamiento;
    } */

    // Cuáles son las direcciones físicas
    dir_fisicas[0] = obtener_frame(dir_logica.pagina) * tam_pagina + dir_logica.desplazamiento;
    for (int i = 1; i < cant_paginas; i++) {
        dir_fisicas[i] = obtener_frame(i+dir_logica.pagina) * tam_pagina;
    }

    t_lista_dir_fisicas lista_dir;
    lista_dir.cant_paginas = cant_paginas;
    lista_dir.dir_fisicas = dir_fisicas;
    lista_dir.bytes_por_pag = bytes_por_pag;

    return lista_dir;
}



int fraccionar_direccion(t_dir_logica dir_logica, int tam_dato) {
    int cant_paginas = 0;
    for (int i = tam_dato + dir_logica.desplazamiento; i > 0; i -= tam_pagina) {
        cant_paginas++;
    }
    return cant_paginas;
}

/* t_list *fraccionar_dato(int dir_logica, void *dato, int tam) {
    t_dir_logica dir_separada = separar_dir_logica(dir_logica);

    // TODO revisar tamaño y buscar frames para usar

    dir_separada.pagina = obtener_frame(dir_separada.pagina);

    t_list *direcciones;
    direcciones = list_create();
    int dir_fisica = obtener_direccion_fisica(dir_separada);
    list_add(direcciones, &dir_fisica);

    return direcciones;
} */

int obtener_frame(int pagina) {
    int frame = tlb_obtener_frame(pcb.pid, pagina);
    if (frame >= 0) {
        return frame;
    }
    else {
        frame = pedir_frame_memoria(pagina);
        tlb_agregar_fila(pcb.pid, pagina, frame);
        return frame;
    }
}



int pedir_frame_memoria(int pagina) {
    t_paquete *paquete = crear_paquete();

    agregar_a_paquete(paquete, &(pcb.pid), sizeof(int));
    agregar_a_paquete(paquete, &pagina, sizeof(int));

    enviar_peticion(paquete, sockets_cpu.socket_memoria, MEM_ACCESO_TABLA_PAGINAS);
    recibir_operacion(sockets_cpu.socket_memoria);
    int frame = recibir_entero(sockets_cpu.socket_memoria);

    t_log *logger = log_create("cpu.log", "cpu", true, LOG_LEVEL_INFO);
    log_info(logger, "PID: %i - OBTENER MARCO - Página: %i - Marco: %i", pcb.pid, pagina, frame);
    log_destroy(logger);

    return frame;
}