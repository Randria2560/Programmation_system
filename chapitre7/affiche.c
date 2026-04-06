#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Fonction exécutée lors de la réception du signal [8, 9]
void gestionnaire(int numero) {
    if (numero == SIGUSR1) { // Vérification du type de signal [7]
        FILE *fp = fopen("/tmp/entier.txt", "r");
        if (fp != NULL) {
            int n;
            fscanf(fp, "%d", &n); // Lecture de l'entier dans le fichier [5]
            printf("\n[NOTIFICATION] L'entier enregistré est : %d\n", n);
            fclose(fp);
            exit(0);
        } else {
            printf("\nErreur : Impossible de lire /tmp/entier.txt\n");
        }
    }
}

int main(void) {
    // Affichage du PID pour permettre la communication avec saisit.c [10]
    printf("Mon PID est : %d\n", getpid());

    struct sigaction action;
    action.sa_handler = gestionnaire; // Association de la fonction de capture [9]
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    // Enregistrement de la capture du signal utilisateur SIGUSR1 [9, 11]
    if (sigaction(SIGUSR1, &action, NULL) != 0) {
        perror("Erreur de configuration du signal");
        exit(1);
    }

    // Boucle d'attente infinie utilisant sleep pour économiser les ressources [1, 12]
    while (1) {
        printf("J'attends que saisit.c m'envoie un signal...\n");
        sleep(10); 
    }

    return 0;
}
