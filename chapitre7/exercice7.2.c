#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char **argv) {

    int pid_dest = atoi(argv[1]);
    int n;

    printf("Veuillez saisir un entier : ");
    scanf("%d", &n);

    FILE *fp = fopen("/tmp/entier.txt", "w");
    if (fp == NULL) { 
        perror("Erreur lors de l'ouverture du fichier"); 
        exit(1); 
    }
    fprintf(fp, "%d", n); 
    fclose(fp);

    // Envoi du signal utilisateur SIGUSR1 au processus affiche.c 
    if (kill(pid_dest, SIGUSR1) == 0) {
        printf("Signal SIGUSR1 envoyé avec succès au PID %d.\n", pid_dest);
    } else {
        perror("Erreur lors de l'envoi du signal");
    }

    return 0;
}
