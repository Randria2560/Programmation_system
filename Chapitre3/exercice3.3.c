//saisis du nom de fichier ne texte et ouvre ce fichier dans l'éditeur emacs dont le fichier se trouve à l'emplacement /usr/bin/emacs
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    char fichier[255];
    printf("Entrer le nom du repertoire:");
    scanf("%s", fichier);
    execl("/usr/bin/vim", "vim", fichier , NULL);
    
    perror("Erreur execv");
    return 1;
}