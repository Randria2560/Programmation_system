/*Écrire un programme qui crée un thread qui alloue un tableau d’entiers,
initialise les éléments par des entiers aléatoires entre 0 et 99, et retourne le tableau d’entiers.*/

#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef struct
{
    int n;
    int *t;
}tableau;


void *ma_fonction(void *arg)
{
    tableau *tab;
    tab->n=5;
    srand(time(NULL));
    tab->t=malloc(tab->n * sizeof(int));
    for(int i=0 ; i<tab->n ; i++)
    {
        tab->t[i] = rand()%99;
        printf("%d ", tab->t[i]);
    }
    return tab;  //retoune pointeur
}

int main()
{
    pthread_t t1;
    void *tableau_original;
    pthread_create(&t1, NULL, ma_fonction,NULL);

    pthread_join(t1, &tableau_original); 

    return 0;
}