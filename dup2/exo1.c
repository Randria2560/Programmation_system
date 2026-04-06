//père écrit sans un fichier via le fils

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int tube[2];
    if (pipe(tube) == -1) { perror("pipe"); exit(1); }

    pid_t fils = fork();
    if (fils == -1) { perror("fork"); exit(1); }

    if(fils == 0)
    {
        close(tube[1]);
    
    }
    else
    {
        close(tube[0]);
        //envoyer la phrase dans le pipe
        char *msg = "Bonjour depuis le père\n";
        write(tube[1], msg , strlen(msg));
        close(tube[1]);

        wait(NULL);
        printf("Fichier resultat.txt créé\n");

    }

    return 0;
}