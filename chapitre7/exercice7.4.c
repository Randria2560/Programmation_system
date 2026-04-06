#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>

// On utilise jmp_buf pour sauvegarder l'état de la pile
// et pouvoir y revenir après un signal
sigjmp_buf point_de_retour;

// Gestionnaire de signal pour SIGSEGV
void gestionnaire_segv(int sig) {
    printf("\n[ERREUR] Erreur de segmentation détectée (i est hors limites) !\n");
    // On retourne au point de sauvegarde défini dans le main
    siglongjmp(point_de_retour, 1);
}

int main() {
    int n, i;
    int *tab;

    // 1. Saisie de la taille et allocation
    printf("Entrez la taille du tableau (n) : ");
    scanf("%d", &n);

    tab = malloc(n * sizeof(int));
    if (tab == NULL) {
        perror("Erreur d'allocation");
        return 1;
    }

    // 2. Remplissage du tableau
    for (int j = 0; j < n; j++) {
        printf("tab[%d] = ", j);
        scanf("%d", &tab[j]);
    }

    // 3. Installation du gestionnaire de signal
    struct sigaction sa;
    sa.sa_handler = gestionnaire_segv;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGSEGV, &sa, NULL);

    // 4. Point de retour en cas de crash
    // sigsetjmp renvoie 0 la première fois, et 1 quand on revient de siglongjmp
    sigsetjmp(point_de_retour, 1);

    // 5. Saisie de l'indice et affichage
    while (1) {
        printf("\nEntrez l'indice i de l'élément à afficher : ");
        if (scanf("%d", &i) != 1) break;

        // Ici, si i est invalide (ex: 1000000), le système génère un SIGSEGV
        // Le gestionnaire interviendra et nous renverra au sigsetjmp plus haut
        printf("Valeur de tab[%d] = %d\n", i, tab[i]);
        
        // Si on arrive ici sans crash, on peut quitter proprement
        break; 
    }

    free(tab);
    printf("Fin du programme sans erreur.\n");
    return 0;
}