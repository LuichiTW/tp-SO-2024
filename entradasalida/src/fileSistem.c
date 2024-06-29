#include "fileSistem.h"

extern t_config_dialfs config_dialfs;
t_bloque *bloques;

//! implementacion aun no confirmada
void carga_archivos_fs(void){
    FILE *bloques;
    FILE *bitmap;
    bloques = fopen("bloques.dat", "r");
    bitmap = fopen("bitmap.dat", "r");
    if (bloques == NULL || bitmap == NULL){
        perror("Error al abrir archivo");
        exit(EXIT_FAILURE);
    }

    //crear bitmap

    //crear bloques


    fclose(bloques);
    fclose(bitmap);
}



//asignacion contigua de bloques
t_bloque *levantar_bloques(void){
    int bloques = open(config_dialfs.path_base_dialfs, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    
    t_bloque *bloques_mapeados;

    bloques_mapeados = mmap(NULL, config_dialfs.block_count * config_dialfs.block_size , PROT_WRITE, MAP_SHARED, bloques, 0);
    if (bloques == MAP_FAILED){
        perror("Error al mapear bloques");
        exit(EXIT_FAILURE);
    }
    bloques_mapeados = msync(bloques, config_dialfs.block_count * config_dialfs.block_size, MS_SYNC);
    close(bloques);

    return bloques_mapeados;
}

void crear_bitmap(void){
    //t_bitarray *bitarray = bitarray_create_with_mode(bitmap, config_dialfs.block_count, LSB_FIRST);


}


