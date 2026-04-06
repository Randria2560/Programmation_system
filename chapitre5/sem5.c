//Producteur et consommateur avec sémaphore:

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t sem;
int compteur=0;
int valeur;

void *producteur(void *arg) 
{
    for(int i=1 ; i<=5 ; i++)
    {
        sem_wait(&sem);     //0
        //protège l'entrée
        valeur=i;
        printf("Le Producteur produit:%d\n", valeur);
        // sleep(2);
        sem_post(&sem);  //1
    }
    return NULL;
}

//0 attend QU'une plaxe se libère

void *consomateur(void *arg) 
{
    for(int i=1; i<=5 ; i++)
    {
        sem_wait(&sem);   //0
        printf("Le consommateur lit :%d\n", valeur);
        sem_post(&sem);    //1
    }
    return NULL;
}

int main() {
    pthread_t tA, tB;

    sem_init(&sem, 0, 1);

    // Créer B AVANT A → normalement B devrait partir en premier
    pthread_create(&tB, NULL, producteur, NULL);
    pthread_create(&tA, NULL, consomateur, NULL);

    pthread_join(tA, NULL);
    pthread_join(tB, NULL);

    sem_destroy(&sem);
    return 0;
}
