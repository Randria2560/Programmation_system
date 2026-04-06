#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define NB_ITERATIONS 10

// sem_t1 : T1 signale à T2 qu'il est arrivé au rendez-vous
// sem_t2 : T2 signale à T1 qu'il est arrivé au rendez-vous
sem_t sem_t1;
sem_t sem_t2;

// ── Thread T1 
void *thread_T1(void *arg) {

    for (int i = 1; i <= NB_ITERATIONS; i++) {
        printf("T1 [iter %d] : début du traitement\n", i);
        sleep(2);
        printf("T1 [iter %d] : traitement terminé, j'attends T2\n", i);

        // ② T1 signale à T2 qu'il est arrivé
        sem_post(&sem_t1);
        // ③ T1 attend que T2 soit arrivé
        sem_wait(&sem_t2);

        // ④ Les deux sont au rendez-vous → on repart
        printf("T1 [iter %d] : rendez-vous OK, on repart !\n\n", i);
    }
    return NULL;
}

void *thread_T2(void *arg) {

    srand(time(NULL));

    for (int i = 1; i <= NB_ITERATIONS; i++) {

        int duree = 4 + rand() % 6;   
        printf("T2 [iter %d] : début du traitement (%d secondes)\n", i, duree);
        sleep(duree);
        printf("T2 [iter %d] : traitement terminé, j'attends T1\n", i);

        // ② T2 signale à T1 qu'il est arrivé
        sem_post(&sem_t2);

        // ③ T2 attend que T1 soit arrivé
        sem_wait(&sem_t1);

        // ④ Les deux sont au rendez-vous → on repart
        printf("T2 [iter %d] : rendez-vous OK, on repart !\n\n", i);
    }

    return NULL;
}

int main() {
    pthread_t t1, t2;

    // bloqué
    sem_init(&sem_t1, 0, 0);
    sem_init(&sem_t2, 0, 0);

    pthread_create(&t1, NULL, thread_T1, NULL);
    pthread_create(&t2, NULL, thread_T2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    
    sem_destroy(&sem_t1);
    sem_destroy(&sem_t2);

    return 0;
}