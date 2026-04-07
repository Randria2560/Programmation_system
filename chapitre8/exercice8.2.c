#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 

#define PORT       33140
#define BUFFER_SIZE 256


int verifier_autorisation(const char *ip_client, char *nom_out, size_t nom_size) {
    FILE *f = fopen("add_autoris.txt", "r");
    if (f == NULL) {
        fprintf(stderr, "Erreur : impossible d'ouvrir add_autoris.txt\n");
        return 0;
    }

    char ligne[BUFFER_SIZE];
    int autorise = 0;

    while (fgets(ligne, sizeof(ligne), f)) {
        ligne[strcspn(ligne, "\n\r")] = '\0';   
        if (strlen(ligne) == 0) continue;       

        char ip_ligne[64]  = "";
        char nom_ligne[128] = "";

       
        sscanf(ligne, "%63s %127s", ip_ligne, nom_ligne);

        if (strcmp(ip_ligne, ip_client) == 0) {
            autorise = 1;
            /* b) Copier le nom si présent */
            if (strlen(nom_ligne) > 0)
                strncpy(nom_out, nom_ligne, nom_size - 1);
            else
                strncpy(nom_out, "inconnu", nom_size - 1);
            nom_out[nom_size - 1] = '\0';
            break;
        }
    }

    fclose(f);
    return autorise;
}


void traite_connection(int sock, const char *nom, const char *ip) {
    char buffer[BUFFER_SIZE];
    char msg[BUFFER_SIZE];
    int nb;

    
    snprintf(msg, sizeof(msg), "Bienvenue %s ! (IP : %s)\n", nom, ip);
    write(sock, msg, strlen(msg));

    write(sock, "Que puis-je faire pour vous ? (tapez 'fin' pour quitter)\n", 57);

    while (1) {
        nb = read(sock, buffer, sizeof(buffer) - 1);
        if (nb <= 0) break;                         
        buffer[nb] = '\0';
        buffer[strcspn(buffer, "\n\r")] = '\0';

        if (strcmp(buffer, "fin") == 0) {
            write(sock, "Au revoir !\n", 12);
            break;
        }

        snprintf(msg, sizeof(msg), "Reçu : %s\n", buffer);
        write(sock, msg, strlen(msg));
    }
}

int cree_socket_serveur() {
    int sock, opt = 1;
    struct sockaddr_in adresse;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }

    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&adresse, 0, sizeof(adresse));
    adresse.sin_family      = AF_INET;
    adresse.sin_port        = htons(PORT);
    adresse.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr*)&adresse, sizeof(adresse)) < 0) {
        perror("bind");
        close(sock);
        return -1;
    }

    return sock;
}


int main(void) {
    int sock_contact, sock_client;
    struct sockaddr_in adresse_client;
    socklen_t longueur = sizeof(adresse_client);

    signal(SIGCHLD, SIG_IGN);   /* Évite les processus zombies */

    sock_contact = cree_socket_serveur();
    if (sock_contact < 0) exit(1);

    if (listen(sock_contact, 5) < 0) {
        perror("listen");
        exit(1);
    }

    while (1) {
        sock_client = accept(sock_contact,
                             (struct sockaddr*)&adresse_client,
                             &longueur);
        if (sock_client < 0) {
            perror("accept");
            continue;
        }

        char ip_client[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &adresse_client.sin_addr,
                  ip_client, sizeof(ip_client));
        printf("Tentative de connexion depuis : %s\n", ip_client);

        if (fork() == 0) {
            close(sock_contact);

            char nom_client[128] = "";

            if (!verifier_autorisation(ip_client, nom_client, sizeof(nom_client))) {
                printf("Accès refusé : %s\n", ip_client);
                write(sock_client, "Accès refusé.\n", 14);
                close(sock_client);
                exit(0);
            }

            printf("Accès autorisé : %s (%s)\n", nom_client, ip_client);

            traite_connection(sock_client, nom_client, ip_client);

            close(sock_client);
            exit(0);
        }

        close(sock_client);
    }

    return 0;
}