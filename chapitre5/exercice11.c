#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define TAILLE_BDD 15   // taille de la base de données

// ── Base de données ─────────────────────────────────────────
unsigned char bdd[TAILLE_BDD];   // initialisée à 0

// ── Synchronisation ─────────────────────────────────────────

// Protège nb_lecteurs
// → 1 seul thread modifie le compteur à la fois
pthread_mutex_t mutex_lecteurs = PTHREAD_MUTEX_INITIALIZER;

// Protège la BDD
// → bloqué quand un rédacteur écrit
// → bloqué quand le 1er lecteur entre (libéré quand dernier sort)
pthread_mutex_t mutex_bdd = PTHREAD_MUTEX_INITIALIZER;

// Compte les lecteurs actifs
int nb_lecteurs = 0;

// ── Statistiques ────────────────────────────────────────────
int nb_lectures  = 0;
int nb_ecritures = 0;
pthread_mutex_t mutex_stats = PTHREAD_MUTEX_INITIALIZER;

// ── Structure arguments ─────────────────────────────────────
typedef struct {
    int id;
} Args;

// ── Afficher la BDD ─────────────────────────────────────────
void afficher_bdd() {
    printf("  BDD : [");
    for (int i = 0; i < TAILLE_BDD; i++) {
        printf("%3d", bdd[i]);
    }
    printf(" ]\n");
}

// ── Thread Lecteur ───────────────────────────────────────────
void *lecteur(void *arg) {
    Args *a  = (Args *)arg;
    int   id = a->id;

    srand(time(NULL) + id);

    while (1) {

        // ① Attendre entre 1 et 3 secondes
        sleep(1 + rand() % 3);

        // ── ENTRER en lecture ──────────────────────────────

        // ② Prendre le mutex pour modifier nb_lecteurs
        pthread_mutex_lock(&mutex_lecteurs);
        nb_lecteurs++;

        // ③ Si c'est le PREMIER lecteur → bloquer les rédacteurs
        if (nb_lecteurs == 1) {
            pthread_mutex_lock(&mutex_bdd);
            // "Je suis le premier lecteur,
            //  je ferme la porte aux rédacteurs"
        }

        pthread_mutex_unlock(&mutex_lecteurs);

        // ── LIRE la BDD ───────────────────────────────────

        // ④ Lire un octet à un emplacement aléatoire
        int index = rand() % TAILLE_BDD;
        unsigned char valeur = bdd[index];

        // ⑤ Mettre à jour les stats
        pthread_mutex_lock(&mutex_stats);
        nb_lectures++;
        pthread_mutex_unlock(&mutex_stats);

        printf("Lecteur  %2d : lit    bdd[%2d] = %3d  "
               "(%d lecteurs actifs)\n",
               id, index, valeur, nb_lecteurs);

        // ── SORTIR de lecture ──────────────────────────────

        // ⑥ Décrémenter le compteur de lecteurs
        pthread_mutex_lock(&mutex_lecteurs);
        nb_lecteurs--;

        // ⑦ Si c'est le DERNIER lecteur → libérer les rédacteurs
        if (nb_lecteurs == 0) {
            pthread_mutex_unlock(&mutex_bdd);
            // "Je suis le dernier lecteur,
            //  j'ouvre la porte aux rédacteurs"
        }

        pthread_mutex_unlock(&mutex_lecteurs);
    }

    return NULL;
}

// ── Thread Rédacteur ─────────────────────────────────────────
void *redacteur(void *arg) {
    Args *a  = (Args *)arg;
    int   id = a->id;

    srand(time(NULL) + id + 100);

    while (1) {

        // ① Attendre entre 1 et 10 secondes
        sleep(1 + rand() % 10);

        // ── ENTRER en écriture ─────────────────────────────

        // ② Attendre qu'il n'y ait AUCUN lecteur
        //    → bloqué tant que nb_lecteurs > 0
        //    (le 1er lecteur a verrouillé mutex_bdd)
        pthread_mutex_lock(&mutex_bdd);

        // ── ÉCRIRE dans la BDD ────────────────────────────

        // ③ Modifier un octet à un emplacement aléatoire
        int index           = rand() % TAILLE_BDD;
        unsigned char avant = bdd[index];
        bdd[index]          = rand() % 256;

        // ④ Mettre à jour les stats
        pthread_mutex_lock(&mutex_stats);
        nb_ecritures++;
        pthread_mutex_unlock(&mutex_stats);

        printf("Rédacteur %2d : écrit  bdd[%2d] : "
               "%3d → %3d\n",
               id, index, avant, bdd[index]);
        afficher_bdd();

        // ── SORTIR d'écriture ──────────────────────────────

        // ⑤ Libérer la BDD pour les autres
        pthread_mutex_unlock(&mutex_bdd);
    }

    return NULL;
}

// ── Thread Statistiques ──────────────────────────────────────
void *statistiques(void *arg) {
    while (1) {
        sleep(15);

        pthread_mutex_lock(&mutex_stats);
        printf("\n════════════════════════════════\n");
        printf("Stats : %d lectures, %d écritures\n",
               nb_lectures, nb_ecritures);
        printf("════════════════════════════════\n\n");
        pthread_mutex_unlock(&mutex_stats);
    }
    return NULL;
}

// ── Main ─────────────────────────────────────────────────────
int main() {
    int N, M;

    // ① Saisir N et M
    printf("Nombre de lecteurs   (N) : ");
    scanf("%d", &N);
    printf("Nombre de rédacteurs (M) : ");
    scanf("%d", &M);

    if (N <= 0 || M <= 0) {
        fprintf(stderr, "N et M doivent être > 0\n");
        return 1;
    }

    // ② Initialiser la BDD à 0
    memset(bdd, 0, sizeof(bdd));

    // ③ Allouer les tableaux
    pthread_t *t_lect = malloc(N * sizeof(pthread_t));
    pthread_t *t_red  = malloc(M * sizeof(pthread_t));
    Args      *args_l = malloc(N * sizeof(Args));
    Args      *args_r = malloc(M * sizeof(Args));

    printf("\n=== Démarrage : %d lecteurs, "
           "%d rédacteurs, BDD %d octets ===\n\n",
           N, M, TAILLE_BDD);

    afficher_bdd();
    printf("\n");

    // ④ Créer les N threads lecteurs
    for (int i = 0; i < N; i++) {
        args_l[i].id = i + 1;
        pthread_create(&t_lect[i], NULL, lecteur, &args_l[i]);
    }

    // ⑤ Créer les M threads rédacteurs
    for (int i = 0; i < M; i++) {
        args_r[i].id = i + 1;
        pthread_create(&t_red[i], NULL, redacteur, &args_r[i]);
    }

    // ⑥ Créer le thread de statistiques
    pthread_t t_stats;
    pthread_create(&t_stats, NULL, statistiques, NULL);

    // ⑦ Attendre (les threads tournent indéfiniment)
    for (int i = 0; i < N; i++) pthread_join(t_lect[i], NULL);
    for (int i = 0; i < M; i++) pthread_join(t_red[i],  NULL);

    // ⑧ Libérer les ressources
    pthread_mutex_destroy(&mutex_lecteurs);
    pthread_mutex_destroy(&mutex_bdd);
    pthread_mutex_destroy(&mutex_stats);
    free(t_lect);
    free(t_red);
    free(args_l);
    free(args_r);

    return 0;
}