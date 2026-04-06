#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    int fd_lire   = atoi(argv[1]);   //3
    int fd_ecrire = atoi(argv[2]);   //4

    close(fd_ecrire);

    char message[255];
    read(fd_lire, message, sizeof(message));
    printf("Message lu: %s\n", message);
    close(fd_lire);

    return 0;

}

//et la version python???