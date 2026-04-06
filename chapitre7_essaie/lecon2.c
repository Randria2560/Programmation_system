//Ce programme intercepte l'interruption clavier (Ctrl-C) pour effectuer une sauvegarde de sécurité dans un fichier avant de quitter proprement
//changer le comportement du processus lors de la reception du signal
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int donnees[8];

void gestionnaire(int numero) {
    FILE *fp;
    if (numero == SIGINT) {
        printf("\nSignal d'interruption, sauvegarde...\n");
        fp = fopen("/tmp/sauve.txt", "w");
        for (int i=0 ; i<5 ; i++) fprintf(fp, "%d ", donnees[i]);
        fclose(fp);
        printf("Sauvegarde terminée, terminaison du processus\n");
        exit(0);
    }
}

int main(void) {
    struct sigaction action;

    action.sa_handler = gestionnaire;  /*pointeur de fonction*/

    //initialisation: options de comportements
    sigemptyset(&action.sa_mask); /*ensemble de signaux vide*/

    action.sa_flags = 0;   /*option par défaut*/
    if(sigaction(SIGINT, &action, NULL) != 0) // Capture de Ctrl-C
    {
        fprintf(stderr, "Erreur de sigaction\n");
        exit(1);
    }

    for (int i=0 ; i<5 ; i++)
    {
        printf("donnees[%d] = ", i);
        scanf("%d", &donnees[i]); getchar();
    }

    char continuer='o';
    while(continuer == 'o')
    {
        puts("zzz...");
        sleep(3);
        for (int i=0 ; i<5 ; i++)
        {
            printf("donnees[%d] = %d ", i, donnees[i]);

        }
        printf("\nVoules-vous continuer ? (o/n) ");
        continuer = getchar();
        getchar();
    
    }
    // ... reste du programme (saisie et boucle) ...
}
