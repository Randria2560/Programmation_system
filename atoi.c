#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void afficheMessage(char *message)
{
    puts(message);
}
extern char **environ;

int main(int argc, char **argv)
{
    int a;
    char s[50];

    printf("Saisissze un chiffre:");
    scanf("%s", s);
    a=atoi(s);
    printf("Vous avez saisi: %d\n", a);

    //================================================
    float x;
    char st[50];
    printf("Saisissez des chiffres (avec un point au milieu) : ");
    scanf("%s", st); /* saisie d'une chaîne de caractères */
    sscanf(st, "%f", &x); /* lecture dans la chaîne */
    printf("Vous avez saisi : %f\n", x);

    //==============================================
    // float m;
    // int b;
    // printf("Saisis un réel:");
    // scanf("%d %f", &b , &m);
    // char chaine[50];
    // sprintf(chaine,"vous avez tapez: m=%f, b=%d", m, b);
    // afficheMessage(chaine);


    //=========================================
    printf("NB parametre:%d", argc-1);
    puts("Longueur:");
    for(int i=1 ; i<argc ; i++)
    {
        printf("%s:%ld\n", argv[i], strlen(argv[i]) );
    }

    //Variable environnement=========================
    char *valeur;
    valeur=getenv("PATH");
    if(valeur != NULL)
        printf("PATH:*%s*", valeur);

    return 0;

}