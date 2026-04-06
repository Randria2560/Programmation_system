/*Ecrire un programme qui prend en argument des noms de répertoire et aﬃche
la liste des ﬁchiers de ces répertoires qui ont une taille supérieure à (à peu près) 1M o avec
l’UID du propriétaire du ﬁchier.*/
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
    DIR *dir;
    struct dirent *ent;
    struct stat st;
    char chemin;

    if (argc < 2) {
        fprintf(stderr, "Usage : %s répertoire1 répertoire2 ...\n", argv);
        exit(1);
    }

    // On boucle sur chaque répertoire passé en argument
    for (int i = 1; i < argc; i++) {
        dir = opendir(argv[i]); 
        if (dir == NULL) {
            perror("Erreur d'ouverture");
            continue;
        }

        printf("\n--- Analyse du répertoire : %s ---\n", argv[i]);
        while ((ent = readdir(dir)) != NULL) {
            sprintf(chemin, "%s/%s", argv[i], ent->d_name);


            if (stat(chemin, &st) == 0) {
                // On vérifie si c'est un fichier ordinaire et si taille > 1 Mo
                // 1 Mo = 1024 * 1024 = 1 048 576 octets
                if (S_ISREG(st.st_mode) && st.st_size > 1048576) {
                    printf("Nom : %s | Taille : %ld octets | UID : %d\n", 
                           ent->d_name, (long)st.st_size, st.st_uid); // [7, 8]
                }
            }
        }
        closedir(dir);
    }
    return 0;
}