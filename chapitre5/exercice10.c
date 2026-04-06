#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define TAILLE_TAMPON 10  

unsigned char tampon[TAILLE_TAMPON];   // le tableau de stockage
int           sommet = 0;              // index du prochain élément

sem_t           sem_vide;    // nombre de places vides  (init = 10)
sem_t           sem_plein;   // nombre de places pleines (init = 0)
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // accès tampon

int nb_produits  = 0;
int nb_consommes = 0;

// ── Structure pour passer les arguments aux threads
typedef struct {
    int id;      // numéro du thread
    int type;    // 0 = producteur, 1 = consommateur
} Args;

// ── Fonction PUSH — ajouter dans la pile
void push(unsigned char valeur) {
    tampon[sommet] = valeur;
    sommet++;
}

// ── Fonction POP — retirer depuis la pile
unsigned char pop() {
    sommet--;
    return tampon[sommet];
}

void *producteur(void *arg) {
    Args *a = (Args *)arg;
    int   id = a->id;

    srand(time(NULL) + id);   // seed différent par thread

    while (1) {

        // ① Attendre entre 1 et 3 secondes
        int duree = 1 + rand() % 3;
        sleep(duree);

        // ② Produire un octet aléatoire
        unsigned char octet = 65 + rand() % 26;   // lettre A-Z

        // ③ Attendre qu'il y ait une place vide
        //    BLOQUÉ si tampon plein (sem_vide = 0)
        sem_wait(&sem_vide);

        // ④ Prendre le mutex pour accéder au tampon
        pthread_mutex_lock(&mutex);

        // ⑤ Déposer l'octet dans le tampon (push)
        push(octet);
        nb_produits++;

        printf("Producteur  %2d : dépose   '%c'  "
               "[tampon : %2d/%d places]\n",
               id, octet, sommet, TAILLE_TAMPON);

        // ⑥ Libérer le mutex
        pthread_mutex_unlock(&mutex);

        // ⑦ Signaler qu'une place est maintenant pleine
        sem_post(&sem_plein);
    }

    return NULL;
}

void *consommateur(void *arg) {
    Args *a = (Args *)arg;
    int   id = a->id;

    srand(time(NULL) + id + 100);   // seed différent par thread

    while (1) {

        // ① Attendre entre 1 et 3 secondes
        int duree = 1 + rand() % 3;
        sleep(duree);

        // ② Attendre qu'il y ait un octet à lire
        //    BLOQUÉ si tampon vide (sem_plein = 0)
        sem_wait(&sem_plein);

        // ③ Prendre le mutex pour accéder au tampon
        pthread_mutex_lock(&mutex);

        // ④ Retirer l'octet depuis le tampon (pop)
        unsigned char octet = pop();
        nb_consommes++;

        printf("Consommateur %2d : retire   '%c'  "
               "[tampon : %2d/%d places]\n",
               id, octet, sommet, TAILLE_TAMPON);

        // ⑤ Libérer le mutex
        pthread_mutex_unlock(&mutex);

        // ⑥ Signaler qu'une place est maintenant vide
        sem_post(&sem_vide);
    }

    return NULL;
}

// Affiche les stats toutes les 10 secondes
void *statistiques(void *arg) {
    while (1) {
        sleep(10);
        printf("\n─── Stats : %d produits, %d consommés ───\n\n",
               nb_produits, nb_consommes);
    }
    return NULL;
}

// ── Main ────────────────────────────────────────────────────
int main() {
    int N, M;
    printf("Nombre de producteurs  (N) : ");
    scanf("%d", &N);
    printf("Nombre de consommateurs (M) : ");
    scanf("%d", &M);

    if (N <= 0 || M <= 0) {
        fprintf(stderr, "N et M doivent être > 0\n");
        return 1;
    }

    //    sem_vide  = TAILLE_TAMPON → toutes les places sont vides
    //    sem_plein = 0             → aucune place n'est pleine
    sem_init(&sem_vide,  0, TAILLE_TAMPON);
    sem_init(&sem_plein, 0, 0);

    // ③ Allouer les tableaux de threads et arguments
    pthread_t *t_prod  = malloc(N * sizeof(pthread_t));
    pthread_t *t_cons  = malloc(M * sizeof(pthread_t));
    Args      *args_p  = malloc(N * sizeof(Args));
    Args      *args_c  = malloc(M * sizeof(Args));

    printf("\n=== Démarrage : %d producteurs, "
           "%d consommateurs, tampon %d ===\n\n",
           N, M, TAILLE_TAMPON);

    // ④ Créer les N threads producteurs
    for (int i = 0; i < N; i++) {
        args_p[i].id   = i + 1;
        args_p[i].type = 0;
        pthread_create(&t_prod[i], NULL, producteur, &args_p[i]);
    }

    // ⑤ Créer les M threads consommateurs
    for (int i = 0; i < M; i++) {
        args_c[i].id   = i + 1;
        args_c[i].type = 1;
        pthread_create(&t_cons[i], NULL, consommateur, &args_c[i]);
    }

    // ⑥ Créer le thread de statistiques
    pthread_t t_stats;
    pthread_create(&t_stats, NULL, statistiques, NULL);

    // ⑦ Attendre (les threads tournent indéfiniment)
    for (int i = 0; i < N; i++) pthread_join(t_prod[i], NULL);
    for (int i = 0; i < M; i++) pthread_join(t_cons[i], NULL);

    // ⑧ Libérer les ressources
    sem_destroy(&sem_vide);
    sem_destroy(&sem_plein);
    pthread_mutex_destroy(&mutex);
    free(t_prod);
    free(t_cons);
    free(args_p);
    free(args_c);

    return 0;
}