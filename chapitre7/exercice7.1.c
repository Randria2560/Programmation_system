#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main()
{
    pid_t pid = fork();

    if (pid == 0)
    {
        while (1)
        {
            printf("Fils en cours d'exécution...\n");
            sleep(1);
        }
    }
    else
    {
        char choix;

        while (1)
        {
            printf("\nMenu:\n");
            printf("s: stop\n");
            printf("r: resume\n");
            printf("q: quit\n");
            printf("Choix: ");

            scanf(" %c", &choix);

            if (choix == 's')
            {
                kill(pid, SIGSTOP);
            }
            else if (choix == 'r')
            {
                kill(pid, SIGCONT);
            }
            else if (choix == 'q')
            {
                kill(pid, SIGKILL);
                break;
            }
        }
    }

    return 0;
}