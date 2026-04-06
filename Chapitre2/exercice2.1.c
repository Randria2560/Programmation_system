#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int main()
{
    pid_t fils;
    fils=fork();
    if( fils == -1)
    {
        perror("Erreur lors de la création de processus");
        exit(1);
    }
    else if(fils == 0)
    {
        printf("Je suis dans le fils: moi=%d,père=%d\n",getpid(),getppid());
    }
    else
    {
        printf("Je suis le père: fils:%d,moi:%d,mon_père:%d\n", fils,getpid(), getppid());
    }

    return 0;
}