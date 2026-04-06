/*Écrire un programme qui crée un thread qui prend en paramètre un tableau
d’entiers et l’aﬃche dans la console.*/
#include <pthread.h>
#include <stdio.h>

typedef struct
{
    int n;
    int *t;
}tableau;


void *ma_fonction(void *arg)
{
    tableau *tab=(tableau *)arg;
    for(int i=0 ; i<tab->n ; i++)
    {
        printf("%d ", tab->t[i]);
    }
     pthread_exit(NULL);
}

int main()
{
     pthread_t t1;
    tableau tableau_original;
    tableau_original.n=4;

    int valeurs[] = {10, 20, 30, 40};
    tableau_original.t = valeurs;

    pthread_create(&t1, NULL, ma_fonction,(void *) &tableau_original);

    pthread_join(t1, NULL); 

    return 0;
}