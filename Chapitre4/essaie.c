#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define BUFFER_SIZE 256

int main(void)
{
    pid_t pid_fils;
    int tube[2];
    unsigned char bufferR[256], bufferW[256];

    puts("Création d'un tube");
    if (pipe(tube) != 0)
    {
        fprintf(stderr, "Erreur dans pipe\n");
        exit(1);
    }
    pid_fils = fork();

    if (pid_fils == -1)
    {
        fprintf(stderr, "Erreur dans fork\n");
        exit(1);
    }
    if (pid_fils == 0) {

        printf("Fermeture entrée dans le fils (pid = %d)\n", getpid());

        close(tube[1]);
        read(tube[0], bufferR, BUFFER_SIZE);

        printf("Le fils (%d) a lu : %s\n", getpid(), bufferR);
    }
    else
    {
        printf("Fermeture sortie dans le père (pid = %d)\n", getpid());
        close(tube[0]);

        sprintf(bufferW, "Message du père (%d) au fils", getpid());

        write(tube[1], bufferW, BUFFER_SIZE);
        //afaka miova:
        //write(tube[1], &valeurW, sizeof(type valeur))
        wait(NULL);
    }

    //lier la sortie tube[0] au tube stdin 0_IN, 1_OUT copie
    //tube nommés: passe par un fichier sur le disque: mkfifo systat= fichier

    return 0;
}
