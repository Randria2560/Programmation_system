#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/* Déclaration et initialisation statique du mutex [5] */
pthread_mutex_t my_mutex = PTHREAD_MUTEX_INITIALIZER;

void* ma_fonction_thread(void *arg);

int main(void) {
    int i;
    pthread_t thread[6];
    srand(time(NULL));

    /* Création de 10 threads [2] */
    for (i=0 ; i<10 ; i++)
        pthread_create(&thread[i], NULL, ma_fonction_thread, (void*)(long)i);

    /* Attente de la fin de tous les threads [2] */
    for (i=0 ; i<10 ; i++)
        pthread_join(thread[i], NULL);

    return 0;
}

void* ma_fonction_thread(void *arg) {
    long num_thread = (long)arg;
    int nombre_iterations, i, j, k, n;
    
    nombre_iterations = rand()%8;
    
    for (i=0 ; i<nombre_iterations ; i++) {
        n = rand()%10000;
        
        /* Verrouillage du mutex avant d'entrer en section critique [2, 3] */
        pthread_mutex_lock(&my_mutex);
        
        printf("Le thread numéro %ld commence son calcul\n", num_thread);
        
        /* Simulation d'un calcul long [2] */
        for (j=0 ; j<n ; j++)
            for (k=0 ; k<n ; k++) { }
            
        printf("Le thread numero %ld a fini son calcul\n", num_thread);
        
        /* Déverrouillage du mutex pour laisser la place aux autres [3, 4] */
        pthread_mutex_unlock(&my_mutex);
    }
    
    pthread_exit(NULL);
}