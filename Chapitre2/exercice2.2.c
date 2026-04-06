//créer 2 fils
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


int main()
{
    pid_t fils1;
    fils1=fork();
    if( fils1 == -1)
    {
        perror("Erreur lors de la création d'un des processus");
        exit(1);
    }
    if(fils1 == 0)
    {
        printf("Je suis dans le fils1 : moi=%d,père=%d\n",getpid(),getppid());
    }
    else  //dans le père
    {
        pid_t fils2;
        fils2=fork();
        if( fils2 == 0)  //dans le fils2
        {
            printf("Je suis dans le fils2 : moi=%d,père=%d\n",getpid(),getppid());
        }
        else
        {
            printf("Je suis le père: fils1:%d,fils2=%d,moi:%d,mon_père:%d\n", fils1,fils2,getpid(), getppid());
        }
        
    }

    return 0;
}