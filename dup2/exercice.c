//qui redirige stdout vers un fichier sortie.txt en utilisant  dup2, pui affiche le texte "Bonjour "avec printf

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main()
{
    int fd = open("sortie.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if( fd == -1 ){ perror("open"); exit(1);}
    
    //rediriger stdout, stderr vers le fichier
    dup2(fd, STDERR_FILENO);
    close(fd); 

    printf("Bonjour depuis le fichier\n");
    fflush(stdout);   //forcer l'écriture dans le fichier

    fprintf(stderr, "Erreur critique\n");
    //rediriger stderr vers un fichier


    return 0;
}