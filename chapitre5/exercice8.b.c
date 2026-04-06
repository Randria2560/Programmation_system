#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define NB_ITERATIONS 10

int N;                       
int compteur = 0;       
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// barriere : bloque les threads jusqu'à ce que tous soient là
sem_t barriere;

// Appelée par chaque thread quand il arrive au rendez-vous
void rendez_vous(int id, int iteration) {

    // ① Prendre le mutex pour modifier le compteur
    pthread_mutex_lock(&mutex);
    compteur++;
    printf("Thread %d [iter %d] : arrivé au RDV (%d/%d)\n",
           id, iteration, compteur, N);

    if (compteur == N) {
        // ② Dernier arrivé → réveille TOUS les threads
        printf("─── Tous arrivés à l'iter %d ! On repart ───\n\n",
               iteration);

        // Remettre le compteur à 0 pour la prochaine itération
        compteur = 0;

        // Libérer N threads bloqués sur sem_wait
        for (int i = 0; i < N; i++) {
            sem_post(&barriere);
        }
    }
    pthread_mutex_unlock(&mutex);

    // ③ Chaque thread attend que le dernier arrive
    sem_wait(&barriere);
}


void *thread_func(void *arg) {
    int id = *(int *)arg;

    // Seed différent pour chaque thread
    srand(time(NULL) + id);

    for (int i = 1; i <= NB_ITERATIONS; i++) {

        // ① Traitement de durée aléatoire entre 1 et 5 secondes
        int duree = 1 + rand() % 5;
        printf("Thread %d [iter %d] : travaille %d secondes\n",id, i, duree);
        sleep(duree);

        // ② Aller au rendez-vous
        rendez_vous(id, i);

        // ③ Repartir après le rendez-vous
        printf("Thread %d [iter %d] : repart !\n", id, i);
    }

    return NULL;
}


int main(int argc, char *argv[]) {

    // Vérifier que N est passé en argument
    if (argc < 2) {
        fprintf(stderr, "Usage : %s <N>\n", argv[0]);
        return 1;
    }

    // Récupérer N depuis argv
    N = atoi(argv[1]);
    if (N <= 0) {
        fprintf(stderr, "N doit être > 0\n");
        return 1;
    }

    // Allouer les threads et leurs identifiants
    pthread_t *threads = malloc(N * sizeof(pthread_t));
    int       *ids     = malloc(N * sizeof(int));

    // Initialiser le sémaphore à 0
    // → tous bloqués jusqu'au dernier arrivé
    sem_init(&barriere, 0, 0);

    // Créer les N threads
    for (int i = 0; i < N; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, thread_func, &ids[i]);
    }

    // Attendre la fin de tous les threads
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    // Libérer les ressources
    sem_destroy(&barriere);
    pthread_mutex_destroy(&mutex);
    free(threads);
    free(ids);

    return 0;
}