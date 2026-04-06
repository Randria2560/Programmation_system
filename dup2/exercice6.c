//sauvegarde et restauration de stdout
/*
sauvegarde du stdout original avec dup
stdout >> log.txt (affiche)
restaure le stdout original avec dup2 (affiche)*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int stdout_original = dup(STDOUT_FILENO);

    // ② Ouvrir le fichier log
    int fd = open("log.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) { perror("open"); exit(1); }

    // ③ Rediriger stdout vers le fichier
    dup2(fd, STDOUT_FILENO);
    close(fd);

    // ④ Écrire dans le fichier
    printf("Ceci va dans le fichier\n");
    fflush(stdout);
    //tsy idina le stdout

    //5 on restaure le stdout
    dup2(stdout_sauvegarde, STDOUT_FILENO);
    close(stdout_sauvegarde);

    printf("Ceci va dans le terminal");

    return 0;
}