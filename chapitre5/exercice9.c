#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define NB_OCTETS 10   // nombre d'octets à échanger


unsigned char boite = 0;   // la variable globale partagée

// sem_vide  : autorise l'émetteur à déposer
//             (1 = boîte vide = émetteur peut déposer)
// sem_plein : autorise le récepteur à lire
//             (0 = boîte vide au départ = récepteur doit attendre)
sem_t sem_vide;
sem_t sem_plein;


void *emetteur(void *arg) {
    srand(time(NULL));

    for (int i = 1; i <= NB_OCTETS; i++) {

        // ① Attendre une durée variable entre 1 et 3 secondes
        int duree = 1 + rand() % 3;
        sleep(duree);

        // ② Attendre que la boîte soit VIDE
        //    bloque si sem_vide = 0 (boîte encore pleine)
        sem_wait(&sem_vide);

        // ③ Déposer l'octet dans la boîte
        boite = (unsigned char)(65 + i);   // A, B, C, D...
        printf("Émetteur  [%2d] : dépose   '%c'  (attend %ds)\n",
               i, boite, duree);

        // ④ Signaler au récepteur que la boîte est PLEINE
        sem_post(&sem_plein);
    }

    return NULL;
}

// ── Récepteur ───────────────────────────────────────────────
void *recepteur(void *arg) {
    srand(time(NULL) + 1);

    for (int i = 1; i <= NB_OCTETS; i++) {

        int duree = 1 + rand() % 3;
        sleep(duree);

        // ② Attendre que la boîte soit PLEINE
        //    bloque si sem_plein = 0 (boîte encore vide)
        sem_wait(&sem_plein);

        // ③ Lire l'octet depuis la boîte
        unsigned char lu = boite;
        printf("Récepteur [%2d] : lit      '%c'  (attend %ds)\n",
               i, lu, duree);

        // ④ Signaler à l'émetteur que la boîte est VIDE
        sem_post(&sem_vide);
    }

    return NULL;
}
int main() {
    pthread_t t_emet, t_recep;

    //    sem_vide  = 1 → boîte vide au départ → émetteur peut déposer
    //    sem_plein = 0 → boîte vide au départ → récepteur doit attendre
    sem_init(&sem_vide,  0, 1);
    sem_init(&sem_plein, 0, 0);

    printf("=== Début de la communication ===\n\n");

    pthread_create(&t_emet,  NULL, emetteur,  NULL);
    pthread_create(&t_recep, NULL, recepteur, NULL);

    pthread_join(t_emet,  NULL);
    pthread_join(t_recep, NULL);

    printf("\n=== Communication terminée ===\n");

    sem_destroy(&sem_vide);
    sem_destroy(&sem_plein);

    return 0;
}