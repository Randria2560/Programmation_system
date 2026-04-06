#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h> 

typedef struct
{
    int n;
    int *t;
    int entier; 
} TypeTableau;

void *generation(void *arg)
{
    TypeTableau *tab = (TypeTableau *)arg;
    printf("\n Génération du tableau...\n");
    for(int i = 0; i < tab->n; i++)
    {
        tab->t[i] = rand() % 100;
    }
    sleep(2); 
    printf("[Thread 1] Génération terminée.\n");
    pthread_exit(NULL);
}

void *t_recherche(void *arg)
{
    TypeTableau *tab = (TypeTableau *)arg;
    long trouve = 0; 
    
    for(int i = 0; i < tab->n; i++)
    { 
        if(tab->t[i] == tab->entier)
        {
            trouve = 1;
            break;
        }
    }
    pthread_exit((void*)trouve);
}

int main()
{
    pthread_t t1, recherche;
    TypeTableau data;
    int nb_elements;
    void* status; 

    srand(time(NULL));

    printf("Combien d'éléments pour le tableau ? ");
    scanf("%d", &nb_elements);
    
    data.n = nb_elements;
    data.t = malloc(nb_elements * sizeof(int));

    pthread_create(&t1, NULL, generation, (void*)&data); 
    pthread_join(t1, NULL);

    printf("Pendant ce temps, entrez la valeur x à rechercher : ");
    scanf("%d", &data.entier);

    pthread_create(&recherche, NULL, t_recherche, (void*)&data);
    
    pthread_join(recherche, &status);
    int result = (int)(long)status; 

    if(result == 1)
    {
        printf("L'entier %d a été trouvé dans le tableau.\n", data.entier);
    }
    else
    {
        printf("L'entier %d n'est pas dans le tableau.\n", data.entier);
    }

    free(data.t);
    return 0;
}