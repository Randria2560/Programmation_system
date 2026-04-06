#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *calculer(void *arg) {
    // Allouer le résultat sur le tas (pas sur la pile !)
    int *resultat = malloc(sizeof(int));
    *resultat = 10 * 10;
    return resultat;   // retourner le pointeur
}

int main() {
    pthread_t thread;
    void *retour;       // pour récupérer la valeur de retour

    pthread_create(&thread, NULL, calculer, NULL);

    // pthread_join récupère la valeur retournée
    pthread_join(thread, &retour);

    int *resultat = (int *)retour;
    printf("Résultat = %d\n", *resultat);

    free(resultat);   // libérer la mémoire
    return 0;
}