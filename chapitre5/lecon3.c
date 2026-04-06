//lit une valeur entière et la retourne au main
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
void *ma_fonction_thread(void *arg)
{
    int resultat;
    printf("Je suis un thread. Veuillez entrer un entier\n");
    scanf("%d", &resultat);
    pthread_exit((void*)resultat); /* termine le thread proprement */
}
int main(void)
{
    int ret;
    pthread_t my_thread;
    ret = pthread_create(&my_thread, NULL,
    ma_fonction_thread, (void*)NULL);
    if (ret != 0)
    {
        fprintf(stderr, "Erreur de création du thread");
        exit (1);
    }
    pthread_join(my_thread, (void*)&ret); {/* on attend la fin du thread */
    printf("Dans le main, ret = %d\n", ret);
    return 0;
}