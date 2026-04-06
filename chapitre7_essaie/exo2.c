//envoyer un signal avec kill
#include <stdio.h>
#include <unistd.h>

int main()
{
    printf("PID: %d\n", getpid());

    while (1)
    {
        printf("En attente...\n");
        sleep(3);
    }
}
