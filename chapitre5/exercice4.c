#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h> 

typedef struct {
    int n;
    int *t;
} TypeTableau;

typedef struct {
    TypeTableau t1;
    TypeTableau t2;
} ParamInclusion;

void *generation(void *arg) {
    TypeTableau *tab = (TypeTableau *)arg;
    printf("Génération d'un tableau de %d éléments...\n", tab->n);
    for(int i = 0; i < tab->n; i++) {
        tab->t[i] = rand() % 100; 
    }
    sleep(1); 
    pthread_exit(NULL);
}

void* t_inclusion(void* arg) {
    ParamInclusion* p = (ParamInclusion*)arg;
    long est_inclus = 1; 

    for (int i = 0; i < p->t1.n; i++) {
        int trouve = 0;
        for (int j = 0; j < p->t2.n; j++) {
            if (p->t1.t[i] == p->t2.t[j]) {
                trouve = 1;
                break; 
            }
        }
        if (trouve == 0) {
            est_inclus = 0; 
            break;
        }
    }
    pthread_exit((void*)est_inclus); 
}

void* t_annulation(void* arg) {
    char c;
    while (1) {
        c = getchar();
        if (c == 'A' || c == 'a') {
            printf("\n[ANNULATION] Arrêt du programme par l'utilisateur.\n");
            exit(0); 
        }
    }
    return NULL;
}

int main() {
    pthread_t t1, t2, recherche, annulation;
    TypeTableau data1, data2;
    int n1, n2;
    void* status; 

    srand(time(NULL));

    do {
        printf("Taille T1 et T2 (T1 doit être <= T2) : ");
        scanf("%d %d", &n1, &n2);
    } while (n1 > n2);
    
    // Nettoyage du tampon pour le futur getchar()
    while (getchar() != '\n'); 

    data1.n = n1;
    data1.t = malloc(n1 * sizeof(int));
    data2.n = n2;
    data2.t = malloc(n2 * sizeof(int));

    pthread_create(&t1, NULL, generation, (void*)&data1); 
    pthread_create(&t2, NULL, generation, (void*)&data2); 
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    ParamInclusion total = {data1, data2};

    pthread_create(&recherche, NULL, t_inclusion, (void*)&total);
    pthread_create(&annulation, NULL, t_annulation, NULL);

    pthread_join(recherche, &status);

    printf("Résultat du calcul : %s\n", (long)status ? "Inclus" : "Non inclus");

    // Libération de la mémoire [4]
    free(data1.t);
    free(data2.t);
   
    return 0;
}