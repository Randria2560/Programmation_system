//créer 5 processus fils avec for et break sans synchronisation
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int i;

int main()
{
    pid_t fils;
    for(i=1; i<=5 ; i++)
    {
        fils=fork();

        if( fils == -1)
        {
            perror("Erreur lors de la création d'un des processus");
            exit(1);
        }
        if(fils == 0) //je suis dans le fils
        {
            printf("Je suis dans le fils%d: moi=%d,père=%d\n",i,getpid(),getppid());
            break;
        }
        // if(fils > 0)  //dans le père
        // {
        //     printf("Je suis dans le père: fils%d =%d, moi=%d, mon_père=%d\n",i, fils , getpid(), getppid());
        // }
    }
    
    return 0;
}