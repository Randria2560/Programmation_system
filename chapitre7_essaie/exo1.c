//intercepter trl+C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler(int sig)
{
    if ( sig == SIGINT)
    {
        printf("\n Signal recu\n");
    }

}


int main()
{
    struct sigaction sa;
    sa.sa_handle= handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0; 
    sigaction(SIGINT,&a, NULL);  //associe ctrl+C à la fonction
    //ou bien : signal(SIGINT, handler);  handle recoit le numéro du signal


    //ignore le signal: signal(SIGINT, SIG_IGN)

    while(1)
    {
        printf("Programme en cours...\n");
        sleep(2);
    }
    return 0;
}