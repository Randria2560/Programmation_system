//Créer un sémaphore initialisé à 1: Deux threads essaient d'entrer l'un apres l'autre

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


sem_t sem;

void *thread_func(void *arg)
{
    int id=*(int *) arg;
    printf("Thread %d : j'attends...\n", id);

    // ① Essayer d'entrer
    sem_wait(&sem);   //devient 0

    // ② Zone protégée
    printf("Thread %d : j'entre !\n", id);
    sleep(2);   // simuler un travail
    printf("Thread %d : je sors.\n", id);

    // ③ Libérer
    sem_post(&sem);

    return NULL;
}

int main()
{
    pthread_t t1, t2;
    int id1 = 1, id2 = 2;

    // Sémaphore = 1 → un seul thread à la fois
    sem_init(&sem, 0, 1);

    pthread_create(&t1, NULL, thread_func, &id1);
    pthread_create(&t2, NULL, thread_func, &id2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    sem_destroy(&sem);
    return 0;
}