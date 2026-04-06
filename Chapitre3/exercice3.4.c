#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

int main() {
    char destination[255];

    while (1) {
        printf("\nEntrez le nom du répertoire "
               "(ou 'q' pour quitter) : ");
        scanf("%s", destination);

        if (strcmp(destination, "q") == 0) {
            break;
        }

        if (mkdir(destination, 0755) == -1) {
            if (errno != EEXIST) {
                perror("mkdir");
                continue;
            }
            printf("Répertoire '%s' existe déjà.\n", destination);
        }

        // Création d'un fils pour effectuer la copie
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            continue;
        }

        if (pid == 0) {
            execlp("cp", "cp", "-r", ".", destination, NULL);
            perror("execlp");
            exit(1);

        } else {
            int status;
            waitpid(pid, &status, 0);

            if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
                printf("Copie vers '%s' terminée.\n", destination);
            else
                printf("Erreur lors de la copie vers '%s'.\n", destination);
        }
    }

    printf("Fin du programme.\n");
    return 0;
}