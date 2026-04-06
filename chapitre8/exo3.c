//serveur multi-clients avec processus
#include <signal.h>
#include <sys/wait.h>


while (1) {
    client_fd = accept(server_fd, NULL, NULL);
    pid = fork();

    if (pid == 0) {           /* Processus fils */
        close(server_fd);      /* Le fils n'écoute pas */
        handle_client(client_fd);
        close(client_fd);
        exit(0);
    } else {                    /* Processus parent */
        close(client_fd);      /* Le parent ne parle pas */
    }
}

/*#include <signal.h>
#include <sys/wait.h>

/* Nettoyage automatique des processus fils terminés 
signal(SIGCHLD, SIG_IGN);
 ou handler explicite : 
void sigchld_handler(int s) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}*/