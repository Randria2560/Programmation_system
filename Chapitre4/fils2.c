#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    char *env_lecture  = getenv("FD_LECTURE");
    char *env_ecriture = getenv("FD_ECRITURE");

    int fd_lire   = atoi(env_lecture);   //3
    int fd_ecrire = atoi(env_ecriture);   //4

    close(fd_ecrire);

    char message[255];
    read(fd_lire, message, sizeof(message));
    printf("Message lu: %s\n", message);
    close(fd_lire);

    return 0;

}