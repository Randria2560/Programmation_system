#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h> 

#define N 100
double tab[N];
pthread_mutex_t mon_mutex = PTHREAD_MUTEX_INITIALIZER;

void *remplace(void *arg)
{
    int nbsec;
    while(1) 
    {
        pthread_mutex_lock(&mon_mutex);
        for(int i=1 ; i<99 ; i++)
        {
            tab[i] = (tab[i-1] + tab[i] + tab[i+1]) / 3.0;
        }
        pthread_mutex_unlock(&mon_mutex);
        
        nbsec = rand() % 3 + 1; // 1 à 3 secondes
        sleep(nbsec);
    }
    return NULL;
}

void *affiche(void *arg)
{
    while(1) 
    {
        pthread_mutex_lock(&mon_mutex); 
        printf("\n--- État du tableau ---\n");
        for(int i=0 ; i<N ; i++)
        {
            printf("Valeur [%d]: %.2f  ", i, tab[i]);
            if (i % 5 == 4) printf("\n"); 
        }
        pthread_mutex_unlock(&mon_mutex);

        sleep(4);
    }
    return NULL;
}

int main()
{
    pthread_t t1, t2;
    srand(time(NULL));

    // Initialisation
    tab[0] = 0;
    tab[99] = 0;
    for(int i=1; i<99 ; i++)
    {
        tab[i] = (double)(rand() % 100);
    }

    // Création des threads [11, 12]
    pthread_create(&t1, NULL, remplace, NULL);
    pthread_create(&t2, NULL, affiche, NULL);

    // Attente des threads (le programme tournera indéfiniment ici) [13]
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}