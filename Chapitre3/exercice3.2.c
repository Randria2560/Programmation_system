#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    char *home = getenv("HOME");
    char nouveau_repertoire;
    char commande_mkdir;

    if (home == NULL) return 1;

    sprintf(nouveau_repertoire, "%s/nouveau_repertoire", home);
    
    sprintf(commande_mkdir, "mkdir %s", nouveau_repertoire);
    system(commande_mkdir);

    char *argv[] = {"cp", "-r", ".", nouveau_repertoire, NULL};
    execv("/usr/bin/cp", argv);
    
    perror("Erreur execv");
    return 1;
}
