#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NB_FILS 5

pid_t pids[NB_FILS];

// Gestionnaire de signal pour le père et les fils
void handle_sigterm(int sig) {
    pid_t self = getpid();
    int is_parent = 0;
    
    for(int i = 0; i < NB_FILS; i++) {
        if (pids[i] == 0) { // On est dans un fils (approximatif ici, mieux vaut vérifier via getppid)
             printf("[Fils %d] meurs\n", self);
             exit(0);
        }
    }
    // Si le code arrive ici, c'est le père
    printf("[Père %d] tué->Fin du programme.\n", self);
    exit(0);
}

void fils_process(int index) {
    // Chaque fils installe son propre gestionnaire pour le message de fin
    signal(SIGTERM, handle_sigterm);
    
    printf("Fils %d (PID: %d) démarré.\n", index, getpid());
    while(1) {
        // Boucle infinie demandée
        sleep(1); 
    }
}

int main() {
    int choix, num_fils;

    // Installation du gestionnaire pour le père
    signal(SIGTERM, handle_sigterm);

    // Création des 5 fils
    for (int i = 0; i < NB_FILS; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {
            fils_process(i);
            exit(0); 
        }
    }

    // Boucle de menu pour le père
    while (1) {
        printf("\n--- MENU PERE (Mon PID: %d) ---\n", getpid());
        printf("1. Endormir un fils (SIGSTOP)\n");
        printf("2. Réveiller un fils (SIGCONT)\n");
        printf("3. Terminer un fils (SIGTERM)\n");
        printf("4. Quitter (Tue le père et les fils)\n");
        printf("Choix : ");
        scanf("%d", &choix);

        if (choix == 4) {
            for(int i=0; i<NB_FILS; i++) kill(pids[i], SIGTERM);
            raise(SIGTERM); // Le père s'envoie le signal à lui-même
        }

        printf("Numéro du fils (0 à %d) : ", NB_FILS - 1);
        scanf("%d", &num_fils);

        if (num_fils < 0 || num_fils >= NB_FILS) {
            printf("Fils invalide.\n");
            continue;
        }

        switch (choix) {
            case 1:
                kill(pids[num_fils], SIGSTOP);
                printf("Fils %d suspendu.\n", num_fils);
                break;
            case 2:
                kill(pids[num_fils], SIGCONT);
                printf("Fils %d réveillé.\n", num_fils);
                break;
            case 3:
                kill(pids[num_fils], SIGTERM);
                printf("Demande de terminaison envoyée au fils %d.\n", num_fils);
                break;
            default:
                printf("Option inconnue.\n");
        }
    }

    return 0;
}