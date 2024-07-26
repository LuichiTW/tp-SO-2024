#include "fileSistem.h"

extern t_config config_dialfs; // ! modificar funciones que usen este parametro para que usen el obterner del archivo config
t_bloque *bloques;

// todo: implementar el ftruncate para que el archivo no supere el tamaño maximo
// todo: eliminar los bloques vacios que superen el maximo de bloques
// todo: implementar el aviso de que el archivo supera el tamaño maximo
// todo: logs principales

void rutina_archivos(void){
    //comprueba si los archivos estan vacios
    //si estan vacios los crea
    //si no estan vacios los carga
}

void compactacion(t_bloque *bloques, t_bitarray *bitmap){
    //compactar bloques
    bloques = compactar_bloque(bloques);

    //actualizar bloques.dat
    guardarListaEnArchivo(bloques, "bloques.dat");

    //actualizar bitmap
    actualizar_bitmap(bitmap, bloques);
    guardar_bitmap(bitmap);

}

//! implementacion aun no confirmada
void carga_archivos_fs(void){
    FILE *bloques;
    FILE *bitmap;
    bloques = fopen("bloques.dat", "rb");//usar ftruncate para crear archivo
    bitmap = fopen("bitmap.dat", "rb");
    if (bloques == NULL || bitmap == NULL){
        perror("Error al abrir archivos");
        exit(EXIT_FAILURE);
    }

    //crear bitmap

    //crear bloques


    fclose(bloques);
    fclose(bitmap);
}

t_bitarray *crear_bitmap(t_config config_dialfs){
    int cantidad_bloques = config_get_int_value(config_dialfs, "CANTIDAD_BLOQUES");
    char *bitarray = malloc(cantidad_bloques);
    t_bitarray *nuevo_bitarray = bitarray_create_with_mode(bitarray, cantidad_bloques, LSB_FIRST);
    return nuevo_bitarray;
}

//actualiza el bitmap con los bloques
void actualizar_bitmap(t_bitarray *bitmap, t_bloque *bloques){
    //manejo de errores
    if (bitmap == NULL || bloques == NULL) {
        fprintf(stderr, "Error: bitmap o bloques es NULL\n");
        return;
    }
    //recorre los bloques y actualiza el bitmap
    t_bloque *temp = bloques;
    int bit_index = 0;
    while (temp != NULL)
    {
        if (bit_index < 0 || bit_index >= bitarray_get_max_bit(bitmap)) {
            fprintf(stderr, "Error: índice de bit fuera de rango\n");
            return; // Indica un error
        }
        if(!string_is_empty(temp->dato)){
            bitarray_set_bit(bitmap, bit_index);
        }else{
            bitarray_clean_bit(bitmap, bit_index);
        }

        bit_index++;
        temp = temp->siguiente;
    }
    //limpieza de bitmap para los bloques no usados
    for (int i = bit_index; i < bitarray_get_max_bit(bitmap); i++)
    {
        bitarray_clean_bit(bitmap, i);
    }
}

//guarda el bitmap en un archivo
void guardar_bitmap(t_bitarray *bitmap){ // ! posible cambio para que use un parametro de nombre de archivo
    FILE *archivo = fopen("bitmap.dat", "wb");
    if (archivo == NULL)
    {
        perror("Error al abrir el archivo");
        return;
    }
    fwrite(bitmap->bitarray, sizeof(char), bitmap->size, archivo);
    fclose(archivo);
}

//carga el bitmap desde un archivo
t_bitarray *cargar_bitmap(void){
    FILE *archivo = fopen("bitmap.dat", "rb");
    if (archivo == NULL)
    {
        perror("Error al abrir el archivo");
        return NULL;
    }
    fseek(archivo, 0, SEEK_END);
    long size = ftell(archivo);
    fseek(archivo, 0, SEEK_SET);
    char *bitarray = malloc(size);
    if (fread(bitarray, sizeof(char), size, archivo) != size)
    {
        perror("Error al leer el archivo");
        free(bitarray);
        fclose(archivo);
        return NULL;
    }
    fclose(archivo);
    t_bitarray *nuevo_bitarray = bitarray_create_with_mode(bitarray, size, LSB_FIRST);
    return nuevo_bitarray;
}


// imprime el bitmap
void imprimir_bitmap(t_bitarray *bitmap){
    for (int i = 0; i < bitmap->size; i++)
    {
        printf("%d", bitarray_test_bit(bitmap, i));
    }
    printf("\n");
}

//asignacion contigua de bloques
t_bloque *levantar_bloques(void){
    t_bloque *bloques_mapeados = leerListaDesdeArchivo("bloques.dat");//cargar bloques desde archivo
    if (bloques_mapeados == NULL){
        perror("Error al cargar bloques");
        exit(EXIT_FAILURE);
    }
    return bloques_mapeados;
}

t_bloque *crear_bloque(size_t tamano_dato) {
    // Asignar memoria para la estructura y el tamaño del dato
    t_bloque *nuevo_bloque = (t_bloque *)malloc(sizeof(t_bloque) + tamano_dato);
    if (nuevo_bloque == NULL) {
        perror("Error al asignar memoria");
        exit(EXIT_FAILURE);
    }
    nuevo_bloque->longitud = tamano_dato;
    nuevo_bloque->siguiente = NULL;
    return nuevo_bloque;
}

t_bloque *insertarAlFinal(t_bloque *cabeza, t_config config_dialfs, char *dato)
{
    int tamano_bloque = config_get_int_value(config_dialfs, "TAMANO_BLOQUE");
    if(*dato){
    while (*dato)//recorre la cadena de caracteres
    {
        t_bloque *nuevoNodo = crear_bloque(tamano_bloque);
        
        strncpy(nuevoNodo->dato, dato, tamano_bloque - 1);
        nuevoNodo->dato[tamano_bloque] = '\0'; // Asegurarse de que la cadena esté terminada
        nuevoNodo->siguiente = NULL;

        // Avanzar el puntero de dato
        dato += strlen(nuevoNodo->dato);

        if (cabeza == NULL)
        {
            cabeza = nuevoNodo;
        }
        else
        {
            t_bloque *temp = cabeza;
            while (temp->siguiente != NULL)
            {
                temp = temp->siguiente;
            }
            temp->siguiente = nuevoNodo;
        }
    }
    }else{
        t_bloque *nuevoNodo = crear_bloque(tamano_bloque);
        
        strncpy(nuevoNodo->dato, dato, tamano_bloque - 1);
        nuevoNodo->dato[tamano_bloque] = '\0'; // Asegurarse de que la cadena esté terminada
        nuevoNodo->siguiente = NULL;

        if (cabeza == NULL)
        {
            cabeza = nuevoNodo;
        }
        else
        {
            t_bloque *temp = cabeza;
            while (temp->siguiente != NULL)
            {
                temp = temp->siguiente;
            }
            temp->siguiente = nuevoNodo;
        }
    
    }
    return cabeza;    
}


void guardarListaEnArchivo(t_bloque *cabeza, char *nombreArchivo)
{
    FILE *archivo = fopen(nombreArchivo, "wb");
    if (archivo == NULL)
    {
        perror("Error al abrir el archivo");
        return;
    }

    t_bloque *temp = cabeza;
    while (temp != NULL)
    {
        // Primero, guarda la longitud de la cadena
        size_t longitud = strlen(temp->dato);
        fwrite(&longitud, sizeof(size_t), 1, archivo);
        // Luego, guarda la cadena
        fwrite(temp->dato, sizeof(char), longitud, archivo);
        temp = temp->siguiente;
    }
    fclose(archivo);
}

// Función para leer la lista desde el archivo
t_bloque *leerListaDesdeArchivo(char *nombreArchivo)
{
    FILE *archivo = fopen(nombreArchivo, "rb");
    if (archivo == NULL)
    {
        perror("Error al abrir el archivo");
        return NULL;
    }

    t_bloque *cabeza = NULL, *ultimo = NULL;
    while (1)
    {
        size_t longitud;
        if (fread(&longitud, sizeof(size_t), 1, archivo) != 1)
            break;

        char *dato = malloc(longitud);
        if (dato == NULL || fread(dato, sizeof(char), longitud, archivo) != longitud)
        {
            free(dato);
            perror("Error al leer el dato");
            break;
        }

        t_bloque *nuevoNodo = crear_bloque(longitud);
        if (nuevoNodo == NULL)
        {
            perror("Error al asignar memoria");
            free(dato);
            break;
        }

        //nuevoNodo->datos.dato = dato;
        strncpy(nuevoNodo->dato, dato, longitud);
        nuevoNodo->siguiente = NULL;

        if (cabeza == NULL)
        {
            cabeza = nuevoNodo;
        }
        else
        {
            ultimo->siguiente = nuevoNodo;
        }
        ultimo = nuevoNodo;
    }

    fclose(archivo);
    return cabeza;
}

// Función para imprimir la lista
void imprimirLista(t_bloque *cabeza)
{
    t_bloque *temp = cabeza;
    while (temp != NULL)
    {
        printf("%s\n", temp->dato);
        temp = temp->siguiente;
    }
}

void liberarLista(t_bloque *cabeza) {
    t_bloque *temp;
    while (cabeza != NULL) {
        temp = cabeza; // Guardar referencia al nodo actual
        cabeza = cabeza->siguiente; // Avanzar al siguiente nodo
        free(temp); // Liberar el nodo actual
    }
}

//quita los bloques vacios
t_bloque *compactar_bloque(t_bloque *bloques){
    t_bloque *bloque_actual = bloques, *bloque_previo = NULL;
    while (bloque_actual != NULL) {
        // Verificar si el dato actual es un string vacío
        if (string_is_empty(bloque_actual->dato)) {
            t_bloque *temp = bloque_actual;
            if (bloque_previo != NULL) {
                bloque_previo->siguiente = bloque_actual->siguiente;
            } else {
                bloques = bloque_actual->siguiente;
            }
            bloque_actual = bloque_actual->siguiente;
            free(temp);
        } else {
            // Solo mover bloque_previo si no se eliminó el nodo actual
            bloque_previo = bloque_actual;
            bloque_actual = bloque_actual->siguiente;
        }
    }
    return bloques;
}