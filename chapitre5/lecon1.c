#include <stdio.h>
#include <stdint.h>   // pour intptr_t

int SaisisEntier(void) {
    int n;
    printf("Veuillez entrer un entier : ");
    scanf("%d", &n);
    return n;
}

void AfficheEntier(void *arg) {
    int n = (intptr_t)arg;   
    printf("L'entier n vaut %d\n", n);
}

void AfficheDecimal(int n) {
    printf("L'entier n vaut %d\n", n);
}

void AfficheHexa(int n) {
    printf("L'entier n vaut %x\n", n);
}

void ExecAffiche(void (*foncAff)(int), int n) {
    foncAff(n);   
}             

int main(void) {
    void (*foncAff)(void *);
    int  (*foncSais)(void);

    int  entier;
    char rep;

    foncSais = SaisisEntier;
    foncAff  = AfficheEntier;

    entier = foncSais();
    foncAff((void *)(intptr_t)entier);   //erreur architecture

    puts("Déci or hexa?");
    rep = getchar();

    if (rep == 'd')
        ExecAffiche(AfficheDecimal, entier);

    if (rep == 'x')
        ExecAffiche(AfficheHexa, entier);

    return 0;
}