//Limiter l'accès à N threads
//3 threads meme temps. Créer 7 threads et montrer que jamails plus de 3 n'entrent en meme temps

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_THREADS  7
#define MAX_ACCES    3

sem_t sem;
int   compteur_actif = 0;    // combien sont dans la ressource
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *utiliser_ressource(void *arg) {
    int id = *(int *)arg;

    //sem=3
    printf("Thread %d : j'attends\n", id);


    sem_wait(&sem);   // entrer 2

    // Incrémenter le compteur hoe iza no miditra
    pthread_mutex_lock(&mutex);
    compteur_actif++;
    printf("Thread %d : j'entre  [%d/%d actifs]\n",
           id, compteur_actif, MAX_ACCES);
    pthread_mutex_unlock(&mutex);

    sleep(2);   // utiliser la ressource

    // Décrémenter le compteur
    pthread_mutex_lock(&mutex);
    compteur_actif--;
    printf("Thread %d : je sors  [%d/%d actifs]\n",
           id, compteur_actif, MAX_ACCES);
    pthread_mutex_unlock(&mutex);

    sem_post(&sem);   // sortir 3

    return NULL;
}

int main() {
    pthread_t threads[MAX_THREADS];
    int ids[MAX_THREADS];

    // Sémaphore = 3 → max 3 threads en même temps
    sem_init(&sem, 0, MAX_ACCES);

    for (int i = 0; i < MAX_THREADS; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, utiliser_ressource, &ids[i]);
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&sem);
    pthread_mutex_destroy(&mutex);
    return 0;
}