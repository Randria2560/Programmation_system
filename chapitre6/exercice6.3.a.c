#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    int n, i, fd;
    printf("Combien d'entiers voulez-vous saisir ? ");
    scanf("%d", &n);

    int *tab = malloc(n * sizeof(int));
    for (i = 0; i < n; i++) {
        printf("Entier %d : ", i + 1);
        scanf("%d", &tab[i]);
    }
    fd = open("tableau.bin", O_WRONLY | O_CREAT | O_TRUNC, 0664);
    
    if (fd == -1) {
        perror("Erreur lors de la création du fichier");
        exit(1);
    }

    /*Sauvegarde binaire du bloc mémoire complet  */
    write(fd, tab, n * sizeof(int));

    printf("Tableau sauvegardé avec succès dans 'tableau.bin'.\n");
    close(fd);
    free(tab);
    return 0;
}
