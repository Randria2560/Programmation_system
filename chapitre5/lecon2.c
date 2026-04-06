#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

//nb secondes passés en arguments
void *ma_fonction_thread(void *arg)
{
    int nbsec = (long)arg;
    printf("Je suis un thread et j'attends %d secondes\n", nbsec);
    sleep(nbsec);
    puts("Je suis un thread et je me termine");
    pthread_exit(NULL); 
}
   
int main(void)
{
    int ret;
    pthread_t my_thread;
    long nbsec;
    time_t t1;
    srand(time(NULL));
    t1 = time(NULL);
    nbsec = rand()%10; 

    ret = pthread_create(&my_thread, NULL,ma_fonction_thread, (void*)nbsec);
    if (ret != 0)
    {
        fprintf(stderr, "Erreur de création du thread");
        exit (1);
    }
    pthread_join(my_thread, NULL); 
    //pas de retour
    printf("Dans le main, nbsec = %ld\n", nbsec);
    printf("Duree de l'operation = %ld\n",(long) time(NULL)-t1);
    return 0;
}




