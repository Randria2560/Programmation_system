#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// ── Variables partagées ────────────────────────────────
int valeur     = 0;      // la valeur produite
int disponible = 0;      // 1 = valeur prête, 0 = consommée

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  produit    = PTHREAD_COND_INITIALIZER;
pthread_cond_t  consomme   = PTHREAD_COND_INITIALIZER;

// ── Producteur ─────────────────────────────────────────
void *producteur(void *arg) {
    for (int i = 1; i <= 5; i++) {
        pthread_mutex_lock(&mutex);

        // Attendre que le consommateur ait lu
        while (disponible == 1) {
            pthread_cond_wait(&consomme, &mutex);
        }

        // Produire
        valeur     = i;
        disponible = 1;
        printf("Producteur produit : %d\n", valeur);

        // Signaler au consommateur
        pthread_cond_signal(&produit);

        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// ── Consommateur ───────────────────────────────────────
void *consommateur(void *arg) {
    for (int i = 1; i <= 5; i++) {
        pthread_mutex_lock(&mutex);

        // Attendre que le producteur produise
        while (disponible == 0) {
            pthread_cond_wait(&produit, &mutex);
        }

        // Consommer
        printf("Consommateur lit   : %d\n", valeur);
        disponible = 0;

        // Signaler au producteur
        pthread_cond_signal(&consomme);

        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t t_prod, t_cons;

    pthread_create(&t_prod, NULL, producteur,   NULL);
    pthread_create(&t_cons, NULL, consommateur, NULL);

    pthread_join(t_prod, NULL);
    pthread_join(t_cons, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&produit);
    pthread_cond_destroy(&consomme);

    return 0;
}