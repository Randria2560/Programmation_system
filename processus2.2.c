//Pointeur de fonction
#include <stdio.h>


int SaisisEntier(void)
{
    int n;
    printf("Veuillez entrer un entier : ");
    scanf("%d", &n);
    return n;
}
void AfficheEntier(int n)
{
    printf("L'entier n vaut %d\(\backslash\)n", n);
}
int main(void)
{
    void (*foncAff)(int); {/* d�claration d'un pointeur foncAff */
    int (*foncSais)(void); {/*d�claration d'un pointeur foncSais */
    int entier;
    foncSais = SaisisEntier; {/* affectation d'une fonction */
    foncAff = AfficheEntier; {/* affectation d'une fonction */
    
    entier = foncSais(); {/* on ex�cute la fonction */
    foncAff(entier); {/* on ex�cute la fonction */

    return 0;
}