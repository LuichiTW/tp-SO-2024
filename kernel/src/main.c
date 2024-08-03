#include <main.h>

sem_t sem;

int main() 
{
    cargar_config();
    levantar_conexiones();
    iniciar_server_io();
    inicializar_colas();

    pthread_t thread_consola;
    pthread_create(&thread_consola, NULL, (void*)iniciar_consola, NULL);
    pthread_detach(thread_consola);

    pthread_t thread_dispatch;
    pthread_create(&thread_dispatch, NULL, (void*)atender_dispatch, NULL);
    pthread_detach(thread_dispatch);

    sem_init(&sem, false, 0);
    sem_wait(&sem);

    return 0;
}
