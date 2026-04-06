#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 256

int main() {
    int tube1[2];  // fils → père (le mot)
    int tube2[2];  // père → fils (le résultat)

    if (pipe(tube1) == -1 || pipe(tube2) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t fils = fork();
    if (fils == -1) { perror("fork"); exit(1); }

    if (fils == 0) {  
        close(tube1[0]);
        close(tube2[1]);

        //Sauvegarder le vrai stdout (terminal)
        int stdout_terminal = dup(STDOUT_FILENO);

        char mot[BUFFER_SIZE];
        printf("Saisissez le mot : ");
        fflush(stdout);
        scanf("%s", mot);

        // ── Envoyer le mot au père via tube
        dup2(tube1[1], STDOUT_FILENO);  // stdout → tube1
        close(tube1[1]);

        printf("%s", mot);
        fflush(stdout);
        close(STDOUT_FILENO);           // signaler EOF au père

        // restauration de stdout stdout vers le terminal
        dup2(stdout_terminal, STDOUT_FILENO);
        close(stdout_terminal);

        int valR;
        read(tube2[0], &valR, sizeof(int));
        close(tube2[0]);

        if (valR == 1)
            printf("Le mot '%s' existe dans le fichier.\n", mot);
        else
            printf("Le mot '%s' n'existe pas dans le fichier.\n", mot);

    } else { 
        close(tube1[1]);
        close(tube2[0]);

        // ── Recevoir le mot du fils via tube1
        dup2(tube1[0], STDIN_FILENO);   // stdin ← tube1
        close(tube1[0]);

        char mot[BUFFER_SIZE];
        scanf("%s", mot);          

        FILE *fichier = fopen("./fichier.txt", "r");
        if (!fichier) { perror("fopen"); exit(1); }

        char ligne[BUFFER_SIZE];
        int valW = 0;

        while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
            if (strstr(ligne, mot) != NULL) {
                valW = 1;
                break;
            }
        }
        fclose(fichier);

        write(tube2[1], &valW, sizeof(int));
        close(tube2[1]);

        wait(NULL);
    }

    return 0;
}