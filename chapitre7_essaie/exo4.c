/*attend un signal
affiche "Signal reçu"*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig)
{
    printf("Signal reçu : %d\n", sig);
}

int main()
{
    signal(SIGINT, handler);

    printf("En attente d'un signal...\n");
    pause();
    printf("Fin du programme\n");

    return 0;
}