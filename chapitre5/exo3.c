#include <stdio.h>
#include <pthread.h>

#define NB_THREADS 1000

int compteur = 0;   // variable partagée
pthread_mutex_t mutex =  PTHREAD_MUTEX_INITIALIZER;

void *incrementer(void *arg) {
    // ⚠️ Pas de protection → race condition !
    pthread_mutex_lock(&mutex);
    compteur++;
    return NULL;
    pthread_mutex_unlock(&mutex);

}

int main() {
    pthread_t threads[NB_THREADS];

    for (int i = 0; i < NB_THREADS; i++) {
        pthread_create(&threads[i], NULL, incrementer, NULL);
    }

    for (int i = 0; i < NB_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Résultat : %d (attendu : %d)\n", compteur, NB_THREADS);
    // ⚠️ Résultat souvent différent de 1000 !

    pthread_mutex_unlock(&mutex);

    return 0;
}