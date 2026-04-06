#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    char nom_fichier[256];

    while (1) {
        // 1. Saisie du nom de fichier
        printf("\nEntrez le nom du fichier à ouvrir (ou 'q' pour quitter) : ");
        scanf("%s", nom_fichier);

        // 2. Condition de sortie
        if (strcmp(nom_fichier, "q") == 0) {
            break;
        }

        // 3. Création d'un processus fils
        // Indispensable car exec remplace le programme en cours
        pid_t pid = fork();

        if (pid == -1) {
            perror("Erreur fork");
            exit(1);
        }

        if (pid == 0) {
            // Dans le processus fils : on se transforme en emacs
            // On utilise le chemin complet vers l'exécutable [5]
            execl("/usr/bin/vim", "vim", nom_fichier, NULL);

            // Si execl échoue (ex: emacs non installé à cet endroit)
            perror("Erreur lors de l'ouverture d'emacs");
            exit(1);
        } else {
            // Dans le processus père : on attend la fermeture d'emacs
            // pour pouvoir saisir le nom du fichier suivant [6]
            wait(NULL);
            printf("Fermeture d'emacs détectée. Prêt pour un nouveau fichier.\n");
        }
    }

    printf("Fin du programme.\n");
    return 0;
}
