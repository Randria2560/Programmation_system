//prendre 2 arguments en ligne de commande
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    int sum=0;
    if(argc != 3)
    {
        perror("Pas d'argument correct");
        exit(1);
    }
    else
    {
        for(int j=0 ; j<argc-1; j++ )
        {
            sum += atoi(argv[j+1]);
        }
        printf("La somme est: %d\n", sum);
    }
    return 0;
}