//synchroniser l'ordre d'execution
/*Forcer le thread B à toujours s'exécuter après le thread A, peu importe l'ordre de création.*/
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem;

void *thread_A(void *arg) {
    printf("Thread A : je travaille\n");
    printf("Thread A : j'ai fini !\n");

    // Signaler à B qu'il peut y aller
    sem_post(&sem);  //0

    return NULL;
}

void *thread_B(void *arg) {
    // Attendre que A ait fini -1 bloquée
    sem_wait(&sem);  

    printf("Thread B : A a fini, je commence\n");
    printf("Thread B : j'ai fini !\n");

    return NULL;
}

int main() {
    pthread_t tA, tB;

    // Sémaphore = 0 → B est bloqué jusqu'à ce que A fasse post()
    sem_init(&sem, 0, 0);//fermé des le départ

    // Créer B AVANT A → normalement B devrait partir en premier
    pthread_create(&tB, NULL, thread_B, NULL);
    pthread_create(&tA, NULL, thread_A, NULL);

    pthread_join(tA, NULL);
    pthread_join(tB, NULL);

    sem_destroy(&sem);
    return 0;
}
