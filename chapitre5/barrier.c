#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

//aucun thread ne peut passer à la phase2 tant qu'on n'a pas terminé la phase1
#define NB_THREADS 3

pthread_barrier_t barriere;

void *travail(void *arg) {
    int num = *(int *)arg;

    // ── Phase 1 ────────────────────────────────────────
    sleep(num);   // simuler un travail de durée variable
    printf("Thread %d : phase 1 terminée\n", num);

    // Attendre que TOUS les threads finissent la phase 1
    pthread_barrier_wait(&barriere);

    printf("--- Thread %d passe en phase 2 ---\n", num);

    // ── Phase 2 ────────────────────────────────────────
    printf("Thread %d : phase 2 terminée\n", num);

    return NULL;
}

int main() {
    pthread_t threads[NB_THREADS];
    int numeros[NB_THREADS];

    // Initialiser la barrière pour NB_THREADS threads
    pthread_barrier_init(&barriere, NULL, NB_THREADS);

    for (int i = 0; i < NB_THREADS; i++) {
        numeros[i] = i + 1;
        pthread_create(&threads[i], NULL, travail, &numeros[i]);
    }

    for (int i = 0; i < NB_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barriere);
    return 0;
}