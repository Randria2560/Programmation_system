#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>

pid_t pid_f;

int main()
{
    // int pid_fils;
    // pid_fils=fork();
    // if (pid_fils == -1)
    // {
    //     puts("erreur");
    //     exit(1);
    // }
    // if( pid_fils == 0)
    // {
    //     printf("Dans le fils:\npère:%d\n", getppid());
    // }
    // else
    // {
    //     printf("Dans le père:\n fils:%d, père:%d, grand-père:%d\n", pid_fils, getpid(), getppid());
    // }

    //wait(): permettre au processus père d'attendre la fin de ses fils avant de se terminer lui meme
    //waitpid(pid_fils, NULL, 0): père en attente de fils

    //le père récupère le code renvoyé par le fils dans la fonction exit
    printf("=======================================\n");
    int status;
    switch (pid_f=fork())
    {
        case -1: perror("Problème dans fork\n");
                 exit(errno);
                 break;

        case 0: puts("Je suis le fils");
                puts("Je retourne le code 3");
                //sleep(2);
                exit(3);
        
        default: puts("Je suis le père");
                 puts("Je récupère le code de retour");
                 waitpid(pid_f,&status,0);
                 printf("Code de sortie du fils: %d: %d\n", pid_f, WEXITSTATUS(status));
                 break;
        
    }


    return 0;
    
}