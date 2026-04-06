//intercepter sigterm , affiche arret propre, quitter proprement
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler(int sig)
{
    if ( sig == SIGTERM)
    {
        printf("\n arret propre\n");
        exit(0);
    }

}

int main()
{
    signal(SIGTERM,handler);
    printf("%d\n", getpid());
    while(1)
    {
        sleep(2);
        printf("ji\n");
    }
}