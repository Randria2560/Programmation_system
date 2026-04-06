#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

int main() {

    srand(time(NULL));

    // ② Mémoriser le temps de départ
    time_t debut = time(NULL);

    // ③ Durées aléatoires entre 1 et 10 secondes
    int duree1 = 1 + rand() % 10;
    int duree2 = 1 + rand() % 10;

    printf("=== Démarrage ===\n");
    printf("Fils 1 va dormir %d secondes\n", duree1);
    printf("Fils 2 va dormir %d secondes\n", duree2);
    printf("Le plus long est le fils %d (%d secondes)\n\n",
           duree1 > duree2 ? 1 : 2,
           duree1 > duree2 ? duree1 : duree2);

    // ④ Créer fils 1
    pid_t pid1 = fork();
    if (pid1 == -1) { perror("fork fils1"); exit(1); }

    if (pid1 == 0) {
        // ── FILS 1 ──
        printf("[Fils 1] démarre (PID = %d)\n", getpid());
        sleep(duree1);
        printf("[Fils 1] terminé après %d secondes\n", duree1);
        exit(0);
    }

    // ⑤ Créer fils 2
    pid_t pid2 = fork();
    if (pid2 == -1) { perror("fork fils2"); exit(1); }

    if (pid2 == 0) {
        // ── FILS 2 
        printf("[Fils 2] démarre (PID = %d)\n", getpid());
        sleep(duree2);
        printf("[Fils 2] terminé après %d secondes\n", duree2);
        exit(0);
    }


    // ⑥ Attendre les deux fils
    // wait() attend LE PREMIER fils qui se termine
    // on attend deux fois pour attendre LES DEUX fils
    for (int i = 0; i < 2; i++) {
        pid_t pid_termine = wait(NULL);

        if (pid_termine == pid1)
            printf("[Père] fils 1 terminé\n");
        else if (pid_termine == pid2)
            printf("[Père] fils 2 terminé\n");
    }

    // ⑦ Calculer la durée totale
    time_t fin          = time(NULL);
    double duree_totale = difftime(fin, debut);

    printf("\n=== Résultat ===\n");
    printf("Durée totale : %.0f secondes\n", duree_totale);
    printf("(le plus long des deux : %d secondes)\n",
           duree1 > duree2 ? duree1 : duree2);

    return 0;
}