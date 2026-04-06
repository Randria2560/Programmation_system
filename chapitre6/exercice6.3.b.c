#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    struct stat st;
    int fd;
    int *tab;

    if (stat("tableau.bin", &st) != 0) {
        perror("Erreur : impossible de lire les infos du fichier");
        exit(1);
    }

    long taille_totale = st.st_size;
    int nb_elements = taille_totale / sizeof(int);

    printf("Taille du fichier détectée : %ld octets (%d entiers)\n", taille_totale, nb_elements);

    tab = (int *)malloc(taille_totale);
    if (tab == NULL) {
        perror("Erreur d'allocation mémoire");
        exit(1);
    }

    fd = open("tableau.bin", O_RDONLY);
    if (fd == -1) {
        perror("Erreur lors de l'ouverture du fichier");
        free(tab);
        exit(1);
    }

    // Contrairement à un fichier texte, on lit directement le flux d'octets dans la mémoire
    if (read(fd, tab, taille_totale) < taille_totale) {
        perror("Erreur lors de la lecture des données");
    }

    //  Affichage pour vérification
    printf("Contenu du tableau chargé :\n");
    for (int i = 0; i < nb_elements; i++) {
        printf("%d ", tab[i]);
    }
    printf("\n");

    // Nettoyage
    close(fd);
    free(tab);
    return 0;
}