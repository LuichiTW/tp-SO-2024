#include "mmu.h"

uint tam_pagina = 32;

t_dir_logica separar_dir_logica(uint dir_logica) {
    t_dir_logica dir_separada;

    dir_separada.pagina = (int) floor((double)(dir_logica / tam_pagina));
    dir_separada.desplazamiento = dir_logica - dir_separada.pagina * tam_pagina;

    return dir_separada;
}

uint obtener_direccion_fisica(t_dir_logica dir_logica) {
    int frame = obtener_frame(dir_logica.pagina);
    return frame*tam_pagina + dir_logica.desplazamiento;
}

t_list *fraccionar_dato(uint dir_logica, void *dato, int tam) {
    t_dir_logica dir_separada = separar_dir_logica(dir_logica);

    // TODO revisar tamaño y buscar frames para usar

    dir_separada.pagina = obtener_frame(dir_separada.pagina);

    t_list *direcciones;
    direcciones = list_create();
    list_add(direcciones, obtener_direccion_fisica(dir_separada));

    return direcciones;
}


// TODO aplicar esto
int obtener_frame(int pagina) {
    t_paquete *paquete = crear_paquete();

    agregar_a_paquete(paquete, pcb->PID, sizeof(int));
    agregar_a_paquete(paquete, pagina, sizeof(int));

    enviar_peticion(paquete, sockets_cpu.socket_memoria, MEM_ACCESO_TABLA_PAGINAS);
    int frame = recibir_entero(sockets_cpu.socket_memoria);
    return frame;
}