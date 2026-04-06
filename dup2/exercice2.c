#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

//rediriger stdin depuis un fichier

int main() {

    //lire depuis un fichier
    int fd = open("entre.txt", O_WRONLY | O_CREAT , 0644);
    write(fd,"Alice34\n",7);
    close(fd);

    //Ouvrir entree.txt en lecture
    int fd_fichier= open("entre.txt", O_RDONLY);
    if( fd_fichier == -1){ perror("open"); exit(1);}


    dup2(fd_fichier,STDIN_FILENO);
    close(fd_fichier);

    char prenom[50];
    scanf("%s", prenom);

    printf("Bonj: %s\n", prenom);
    return 0;
}