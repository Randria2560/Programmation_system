//compteur de signaux
/*compte combien de fois ctrlC est préssé et quitte après 5 fois*/

/*attend un signal
affiche "Signal reçu"*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
int compteur=0;

void handler(int sig)
{
    compteur++;
    printf("Signal reçu : %d/%d\n", sig, compteur);
    if(compteur >=5)
    {
        printf("Fin\n");
        exit(0);
    }
}

int main()
{
    signal(SIGINT, handler);
    while(1)
    {
        pause();
    }

    
    return 0;
}