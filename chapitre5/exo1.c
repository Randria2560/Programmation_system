#include <pthread.h>
#include <stdio.h>

// Variable partagée par tous les threads [3]
int compteur = 0; 

// Initialisation statique du mutex [5]
pthread_mutex_t mon_mutex = PTHREAD_MUTEX_INITIALIZER;


void* ma_fonction(void* arg) {
    for (int i = 0; i < 1000; i++) {
        // 1. Verrouiller le mutex avant d'accéder à la donnée [5, 6]
        pthread_mutex_lock(&mon_mutex); 
        
        // 2. Section critique : modification de la variable partagée
        compteur++; 
        
        // 3. Déverrouiller le mutex après l'accès [6, 7]
        pthread_mutex_unlock(&mon_mutex); 
    }
    pthread_exit(NULL); // Terminaison propre du thread [8]
}


int main() {
    pthread_t t1, t2;

    // Création de deux threads exécutant la même fonction [9]
    pthread_create(&t1, NULL, ma_fonction, NULL);
    pthread_create(&t2, NULL, ma_fonction, NULL);

    // Attente de la fin des threads (similaire à wait pour les processus) [10, 11]
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Valeur finale du compteur : %d\n", compteur);
    return 0;
}
