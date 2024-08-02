#include "fileSistem.h"


// todo: eliminar los bloques vacios que superen el maximo de bloques
// todo: implementar el aviso de que el archivo supera el tamaño maximo
// todo: logs principales



char *buscar_metadata(int posicion){
    t_metadata *aux = metadata; //lista metadata
    while(aux->bloque_inicial != posicion){
        aux = aux->siguiente;
    }
    if(aux != NULL){
        return aux->nombre;
    }
    return NULL;
}

void comprobar_filesystem(t_config_interfaz *config_dialfs)
{
    char *path_bloques = string_from_format("%s%s", config_dialfs->path_base_dialfs, "/bloques.dat");
    // comprueba si el archivo de bloques esta vacio
    int vacio = archivo_esta_vacio(path_bloques);
    if (vacio == -1)
    {
        // Manejar el error
        exit(EXIT_FAILURE);
    }
    else if (vacio)
    {
        // Si está vacío, lo crea
        FILE *archivo = fopen(path_bloques, "w");
        if (archivo == NULL)
        {
            perror("Error al crear el archivo");
            exit(EXIT_FAILURE);
        }
        ftruncate(fileno(archivo), config_dialfs->block_count * config_dialfs->block_size);
        fclose(archivo);
        // creo los bloques

    }
    else
    {
        // Si no está vacío, lo carga
        bloques = levantar_bloques(config_dialfs);
    }

    char *path_bitmap = string_from_format("%s%s", config_dialfs->path_base_dialfs, "/bitmap.dat");
    // comprueba si el archivo de bitmap esta vacio
    vacio = archivo_esta_vacio(path_bitmap);
    if (vacio == -1)
    {
        // Manejar el error
        exit(EXIT_FAILURE);
    }
    else if (vacio)
    {
        // Si está vacío, lo crea
        FILE *archivo = fopen(path_bitmap, "w");
        if (archivo == NULL)
        {
            perror("Error al crear el archivo");
            exit(EXIT_FAILURE);
        }
        fclose(archivo);
        // creo el bitmap
        bitmap = crear_bitmap(config_dialfs);
    }
    else
    {
        // Si no está vacío, lo carga
        bitmap = cargar_bitmap(config_dialfs);
    }

    //comprueba si hay metadata
    //todo: implementar
    //sino esta lo carga
    metadata = cargar_metadata(config_dialfs);

    free(path_bloques);
    free(path_bitmap);
}

int archivo_esta_vacio(char *nombre_archivo)
{
    struct stat st;
    if (stat(nombre_archivo, &st) != 0)
    {
        perror("Error al obtener información del archivo");
        return -1; // Error al obtener información del archivo
    }
    return st.st_size == 0;
}

void compactacion(t_bloque *bloques, t_bitarray *bitmap)
{
    // compactar bloques
    bloques = compactar_bloque(bloques);

    // actualizar bloques.dat
    guardarListaEnArchivo(bloques, "bloques.dat");// todo: que se pueda pasar el path desde parametro
    
    //actualizar metadata
    compactacion_metadata();
    metadata = cargar_metadata(config_interfaz);

    // actualizar bitmap
    actualizar_bitmap(bitmap, bloques);
    guardar_bitmap(bitmap, config_interfaz);

}

void compactacion_metadata()
{
    
    int l = 0;
    while ((l < config_interfaz->block_count))
    {
        int i = 0;
        while ((bitarray_test_bit(bitmap, i) != 0) && (i < config_interfaz->block_count))
        {
            i++;
        }
        if (bitarray_test_bit(bitmap, i) == 0)
        {
            int j = i;
            while ((bitarray_test_bit(bitmap, j) == 0) && (j < config_interfaz->block_count))
            {
                j++;
            }
            if (bitarray_test_bit(bitmap, j) != 0)
            {
                char *archivo = buscar_metadata(j);
                modificar_metadata(archivo, "BLOQUE_INICIAL", i);
            }
        }
        l++;
    }
}

t_bitarray *crear_bitmap(t_config_interfaz *config_dialfs)
{
    int cantidad_bloques = config_dialfs->block_count;
    char *bitarray = malloc(cantidad_bloques);
    t_bitarray *nuevo_bitarray = bitarray_create_with_mode(bitarray, cantidad_bloques, LSB_FIRST);
    return nuevo_bitarray;
}

// actualiza el bitmap con los bloques
void actualizar_bitmap(t_bitarray *bitmap, t_bloque *bloques)
{
    // manejo de errores
    if (bitmap == NULL || bloques == NULL)
    {
        fprintf(stderr, "Error: bitmap o bloques es NULL\n");
        return;
    }
    // recorre los bloques y actualiza el bitmap
    t_bloque *temp = bloques;
    int bit_index = 0;
    while (temp != NULL)
    {
        if (bit_index < 0 || bit_index >= bitarray_get_max_bit(bitmap))
        {
            fprintf(stderr, "Error: índice de bit fuera de rango\n");
            return; // Indica un error
        }
        if (!string_is_empty(temp->dato))
        {
            bitarray_set_bit(bitmap, bit_index);
        }
        else
        {
            bitarray_clean_bit(bitmap, bit_index);
        }

        bit_index++;
        temp = temp->siguiente;
    }
    // limpieza de bitmap para los bloques no usados
    for (int i = bit_index; i < bitarray_get_max_bit(bitmap); i++)
    {
        bitarray_clean_bit(bitmap, i);
    }
}

// guarda el bitmap en un archivo
void guardar_bitmap(t_bitarray *bitmap, t_config_interfaz *config_dialfs)
{
    char *path_bitmap = string_from_format("%s%s", config_dialfs->path_base_dialfs, "/bitmap.dat");
    FILE *archivo = fopen(path_bitmap, "wb");
    if (archivo == NULL)
    {
        perror("Error al abrir el archivo");
        return;
    }
    fwrite(bitmap->bitarray, sizeof(char), bitmap->size, archivo);
    fclose(archivo);
}

// carga el bitmap desde un archivo
t_bitarray *cargar_bitmap(t_config_interfaz *config_dialfs)
{
    char *path_bitmap = string_from_format("%s%s", config_dialfs->path_base_dialfs, "/bitmap.dat");
    FILE *archivo = fopen(path_bitmap, "rb");
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
void imprimir_bitmap(t_bitarray *bitmap)
{
    for (int i = 0; i < bitmap->size; i++)
    {
        printf("%d", bitarray_test_bit(bitmap, i));
    }
    printf("\n");
}

void insertar_archivo_bloques(t_bloque *archivo){

    if (bloques == NULL) {
        // Si la primera lista está vacía, simplemente apunta a la segunda lista
        bloques = archivo;
        return;
    }
    
    t_bloque *temp = bloques;
    while (temp->siguiente != NULL)
    {
        temp = temp->siguiente;
    }
    temp->siguiente = archivo;

}

// Función para extraer una parte de la lista
t_bloque* extraer_parte_lista(t_bloque *cabeza, int indice_inicial, int cantidad) {
    if (cabeza == NULL || cantidad <= 0) {
        return NULL;
    }

    t_bloque *temp = cabeza;
    t_bloque *prev = NULL;
    t_bloque *inicio_extraccion = NULL;
    t_bloque *fin_extraccion = NULL;

    // Avanzar hasta el índice inicial
    for (int i = 0; i < indice_inicial && temp != NULL; i++) {
        prev = temp;
        temp = temp->siguiente;
    }

    // Si no se encuentra el índice inicial, retornar NULL
    if (temp == NULL) {
        return NULL;
    }

    // Marcar el inicio de la extracción
    inicio_extraccion = temp;

    // Avanzar hasta el final de la cantidad a extraer
    for (int i = 0; i < cantidad && temp != NULL; i++) {
        fin_extraccion = temp;
        temp = temp->siguiente;
    }

    // Ajustar los punteros para mantener la integridad de la lista original
    if (prev != NULL) {
        prev->siguiente = temp;
    } else {
        cabeza = temp;
    }

    // Terminar la sublista extraída
    if (fin_extraccion != NULL) {
        fin_extraccion->siguiente = NULL;
    }

    return inicio_extraccion;
}

int crear_archivo_bloques(t_bloque *cabeza){
    //carga un dato vacio en el bloque
    char *dato = "";
    t_bloque *nuevoNodo = crear_bloque(strlen(dato));
    for (int i = 0; i < bitmap->size; i++)
    {
        if (!bitarray_test_bit(bitmap, i))
        {
            bitarray_set_bit(bitmap, i);
            nuevoNodo->siguiente = NULL;
            cabeza = nuevoNodo;
            return i;
        }
    }
    return -1;
}

void eliminar_archivo_bloques(t_bloque *cabeza, int bloque_inicial, int tamanio){
    t_bloque *temp = cabeza;
    t_bloque *prev = NULL;
    int i = 0;

    // Avanzar hasta el bloque inicial
    while (temp != NULL && i < bloque_inicial) {
        prev = temp;
        temp = temp->siguiente;
        i++;
    }

    // Liberar los bloques desde el bloque inicial hasta el tamaño especificado
    for (int j = 0; j < tamanio && temp != NULL; j++) {
        t_bloque *a_eliminar = temp;
        temp = temp->siguiente;
        bitarray_clean_bit(bitmap, bloque_inicial + j);
        free(a_eliminar);
    }

    // Conectar el nodo anterior al último nodo liberado con el siguiente nodo
    if (prev != NULL) {
        prev->siguiente = temp;
    } else {
        // Si el bloque inicial es el primero, actualizar la cabeza
        cabeza = temp;
    }
}

// asignacion contigua de bloques
t_bloque *levantar_bloques(t_config_interfaz *config_dialfs)
{
    char *path_bloques = string_from_format("%s%s", config_dialfs->path_base_dialfs, "/bloques.dat");
    t_bloque *bloques_mapeados = leerListaDesdeArchivo(path_bloques); // cargar bloques desde archivo
    if (bloques_mapeados == NULL)
    {
        perror("Error al cargar bloques");
        exit(EXIT_FAILURE);
    }
    return bloques_mapeados;
}

t_bloque *crear_bloque(size_t tamano_dato)
{
    // Asignar memoria para la estructura y el tamaño del dato
    t_bloque *nuevo_bloque = (t_bloque *)malloc(sizeof(t_bloque) + tamano_dato);
    if (nuevo_bloque == NULL)
    {
        perror("Error al asignar memoria");
        exit(EXIT_FAILURE);
    }
    nuevo_bloque->longitud = tamano_dato;
    nuevo_bloque->siguiente = NULL;
    return nuevo_bloque;
}

t_bloque *insertarEnLista(t_bloque *cabeza, t_bitarray *bitmap, char *dato){
    //! revisar que funcione
    //todo que busque un espacio vacio en el bitmap y lo asigne
    //todo que si no hay espacio, devuelva un error
    t_bloque *nuevoNodo = crear_bloque(strlen(dato));
    strncpy(nuevoNodo->dato, dato, strlen(dato));
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
    actualizar_bitmap(bitmap, cabeza);
    return cabeza;
}

t_bloque *insertarAlFinal(t_bloque *cabeza, t_config *config_dialfs, char *dato)
{
    int tamano_bloque = config_get_int_value(config_dialfs, "TAMANO_BLOQUE");
    if (*dato)
    {
        while (*dato) // recorre la cadena de caracteres
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
    }
    else
    {
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

        // nuevoNodo->datos.dato = dato;
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

void liberarLista(t_bloque *cabeza)
{
    t_bloque *temp;
    while (cabeza != NULL)
    {
        temp = cabeza;              // Guardar referencia al nodo actual
        cabeza = cabeza->siguiente; // Avanzar al siguiente nodo
        free(temp);                 // Liberar el nodo actual
    }
}

// quita los bloques vacios
t_bloque *compactar_bloque(t_bloque *bloques)
{
    t_bloque *bloque_actual = bloques, *bloque_previo = NULL;
    while (bloque_actual != NULL)
    {
        // Verificar si el dato actual es un string vacío
        if (string_is_empty(bloque_actual->dato))
        {
            t_bloque *temp = bloque_actual;
            if (bloque_previo != NULL)
            {
                bloque_previo->siguiente = bloque_actual->siguiente;
            }
            else
            {
                bloques = bloque_actual->siguiente;
            }
            bloque_actual = bloque_actual->siguiente;
            free(temp);
        }
        else
        {
            // Solo mover bloque_previo si no se eliminó el nodo actual
            bloque_previo = bloque_actual;
            bloque_actual = bloque_actual->siguiente;
        }
    }
    return bloques;
}
t_metadata *cargar_metadata(t_config_interfaz *config_dialfs)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(config_dialfs->path_base_dialfs);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (string_starts_with(dir->d_name, "metadata_"))
            {
                t_metadata *nuevo = malloc(sizeof(t_metadata));
                nuevo->nombre = dir->d_name;
                insertar_a_lista(nuevo);
            }
        }
        closedir(d);
    }
    return metadata;
}

