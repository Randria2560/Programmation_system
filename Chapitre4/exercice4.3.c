#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main()
{
    int tube[2];
    if(pipe(tube) == -1){ perror("pipe"); exit(1);}

    pid_t fils = fork();
    if(fils == -1) { perror("fork"); exit(1);}

    if(fils == 0)
    {
        // Convertir les numéros en texte pour argv
        char lecture[10], ecriture[10];
        sprintf(lecture,  "%d", tube[0]);  // 3 → "3"
        sprintf(ecriture, "%d", tube[1]);  // 4 → "4"

        char *args[] = {"./fils", lecture, ecriture, NULL};
        execv("./fils", args);

        perror("execv"); 
        exit(1);
    }
    else
    {
        close(tube[0]); //le père ne lit pas
        char *message="Bonjour tout le monde: ceci est le message";
        write(tube[1], message ,strlen(message)+1);
        close(tube[1]);

        wait(NULL);

    }
    return 0;
}