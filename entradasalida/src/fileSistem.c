#include "fileSistem.h"

t_config_dialfs config_dialfs;

void carga_archivos_fs(void){
    FILE *bloques;
    FILE *bitmap;
    bloques = fopen(config_dialfs.path_base_dialfs, "r");
    bitmap = fopen("bitmap.dat", "r");
    if (bloques == NULL || bitmap == NULL){
        perror("Error al abrir archivo");
        exit(EXIT_FAILURE);
    }
    fclose(bloques);
    fclose(bitmap);
}

void levantar_bloques(void){
    int bloques = open(config_dialfs.path_base_dialfs, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    
    int bloques_mapeados;

    bloques_mapeados = mmap(NULL, config_dialfs.block_count * config_dialfs.block_size , PROT_WRITE, MAP_SHARED, bloques, 0);
    if (bloques == MAP_FAILED){
        perror("Error al mapear bloques");
        exit(EXIT_FAILURE);
    }
    bloques_mapeados = msync(bloques, config_dialfs.block_count * config_dialfs.block_size, MS_SYNC);
    close(bloques);
}

