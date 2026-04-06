#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int compteur = 0; 
pthread_mutex_t mon_mutex = PTHREAD_MUTEX_INITIALIZER;

void* incrementation(void* arg) {
    int limite = (int)(long)arg;
    while (1) {
        pthread_mutex_lock(&mon_mutex);
        if (compteur >= limite) {
            pthread_mutex_unlock(&mon_mutex);
            break;
        }
        compteur++; 
        pthread_mutex_unlock(&mon_mutex);
        
        sleep(rand() % 5 + 1);
    }
    pthread_exit(NULL); 
}

void* affiche(void* arg) {
    int limite = (int)(long)arg;
    while (1) {
        pthread_mutex_lock(&mon_mutex);
        if (compteur >= limite) {
            pthread_mutex_unlock(&mon_mutex);
            break;
        }
        printf("Valeur actuelle du compteur : %d\n", compteur);
        pthread_mutex_unlock(&mon_mutex);
        
        sleep(2); // Affichage toutes les 2 secondes [1]
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;
    int limite = atoi(argv[1]);
    pthread_t t1, t2;
    srand(time(NULL));

    pthread_create(&t1, NULL, incrementation, (void*)(long)limite);
    pthread_create(&t2, NULL, affiche, (void*)(long)limite);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Fin du programme. Compteur final : %d\n", compteur);
    return 0;
}
