#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h> 
#include <math.h>

#define N 100
int compte=0;
double u;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int nbsec=0;

void *thread1(void *arg)
{ 
    while(1)
    {
        pthread_mutex_lock(&mutex);
        compte +=1;
        u=(1.0/4.0)*pow(u-1,2);
        printf("La valeur de u dans 1: %lf\n",u);
        nbsec=rand()%5+1;
        pthread_mutex_unlock(&mutex);
        sleep(nbsec);

    }  
    return NULL; 
}

void *thread2(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        compte +=1;
        u=(1.0/6.0)*pow(u-2,2);
        printf("La valeur de u dans 2: %lf\n",u);
        nbsec=rand()%5+1;
        pthread_mutex_unlock(&mutex);
        sleep(nbsec);

    }
    return NULL;
}

int main()
{
    pthread_t t1, t2;
    srand(time(NULL));
    u=1;

    // Création des threads [11, 12]
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    // Attente des threads (le programme tournera indéfiniment ici) [13]
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&mutex);

    return 0;
}