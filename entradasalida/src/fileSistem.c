#include "fileSistem.h"

extern t_config_dialfs config_dialfs;
t_bloque *bloques;

void compactacion(t_bloque *bloques, t_bitarray *bitmap){
    //compactar bloques
    bloques = compactar_bloque(bloques);

    //actualizar bloques.dat

    //actualizar bitmap

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

//asignacion contigua de bloques
t_bloque *levantar_bloques(void){
    t_bloque *bloques_mapeados = leerListaDesdeArchivo("bloques.dat");//cargar bloques desde archivo
    if (bloques_mapeados == NULL){
        perror("Error al cargar bloques");
        exit(EXIT_FAILURE);
    }
    return bloques_mapeados;
}


//!falta completar
void crear_bitmap(void){
    //t_bitarray *bitarray = bitarray_create_with_mode(bitmap, config_dialfs.block_count, LSB_FIRST);


}


t_bloque *insertarAlFinal(t_bloque *cabeza, char *dato)
{
    if(*dato){
    while (*dato)//recorre la cadena de caracteres
    {
        t_bloque *nuevoNodo = malloc(sizeof(t_bloque));
        if (nuevoNodo == NULL)
        {
            perror("Error al asignar memoria para nuevoNodo");
            return cabeza;
        }
        strncpy(nuevoNodo->dato, dato, 1023);
        nuevoNodo->dato[1024] = '\0'; // Asegurarse de que la cadena esté terminada
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
        t_bloque *nuevoNodo = malloc(sizeof(t_bloque));
        if (nuevoNodo == NULL)
        {
            perror("Error al asignar memoria para nuevoNodo");
            return cabeza;
        }
        strncpy(nuevoNodo->dato, dato, 1023);
        nuevoNodo->dato[1024] = '\0'; // Asegurarse de que la cadena esté terminada
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


void guardarListaEnArchivo(t_bloque *cabeza, const char *nombreArchivo)
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

        t_bloque *nuevoNodo = (t_bloque *)malloc(sizeof(t_bloque));
        if (nuevoNodo == NULL)
        {
            perror("Error al asignar memoria");
            free(dato);
            break;
        }

        //nuevoNodo->datos.dato = dato;
        strncpy(nuevoNodo->dato, dato, 1024);
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
        if (bloque_actual->dato[0] == '\0') {
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