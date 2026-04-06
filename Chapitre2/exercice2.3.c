//créer 5 processus fils avec for et break sans synchronisation
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


int main()
{
    pid_t fils[5];
    for(int i=1; i<=5 ; i++)
    {
        fils[i]=fork();
        if( fils[i] == -1)
        {
            perror("Erreur lors de la création d'un des processus");
            exit(1);
        }
        if(fils[i] == 0) //je suis dans le fils
        {
            printf("Je suis dans le fils[%d] : moi=%d,père=%d\n",i,getpid(),getppid());
            break;
        }
    }

    return 0;
}