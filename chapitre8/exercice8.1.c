#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFFER_SIZE 255
#define MAX_PRODUITS 100

typedef struct {
    char nom[32];
    float prix;
} Produit;

Produit catalogue[MAX_PRODUITS] = {
    {"pomme", 0.50},
    {"banane", 0.30},
    {"orange", 0.75}
};
int nb_produits = 3;

Produit* recherche_produit(char *nom) {
    for (int i = 0; i < nb_produits; i++) {
        if (strcmp(catalogue[i].nom, nom) == 0) {
            return &catalogue[i];
        }
    }
    return NULL;
}

/* f) Crée un fichier de commande avec nom unique basé sur la date+heure+pid */
FILE* ouvrir_fichier_commande() {
    char nom_fichier[64];
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(nom_fichier, sizeof(nom_fichier),
             "commande_%Y%m%d_%H%M%S_", tm_info);
    char pid_str[16];
    sprintf(pid_str, "%d.txt", getpid());
    strcat(nom_fichier, pid_str);
    return fopen(nom_fichier, "w");
}

int cree_socket_tcp_ip() {
    int sock, opt = 1;
    struct sockaddr_in adresse;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) return -1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    memset(&adresse, 0, sizeof(struct sockaddr_in));
    adresse.sin_family = AF_INET;
    adresse.sin_port = htons(33140);
    adresse.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sock, (struct sockaddr*) &adresse, sizeof(struct sockaddr_in)) < 0) return -1;
    return sock;
}

void traite_connection(int sock) {
    char bufferR[BUFFER_SIZE], bufferW[BUFFER_SIZE];
    int nb, quantite;
    float total_commande = 0.0;

    
    FILE *fichier = ouvrir_fichier_commande();
    if (fichier == NULL) {
        write(sock, "Erreur serveur : impossible de créer le fichier.\n", 50);
        return;
    
    time_t t = time(NULL);
    fprintf(fichier, "=== COMMANDE DU %s", ctime(&t));
    fprintf(fichier, "%-20s %-10s %-10s\n", "Produit", "Quantite", "Sous-total");
    fprintf(fichier, "--------------------------------------------\n");

    write(sock, "=== Nouvelle commande ===\n", 25);
    write(sock, "(Tapez 'fin' comme nom de produit pour terminer)\n\n", 50);

    /* multi-produit*/
    while (1) 
    {
        write(sock, "Nom du produit : ", 17);
        nb = read(sock, bufferR, BUFFER_SIZE);
        if (nb <= 0) break;
        if (nb >= 2) bufferR[nb-2] = '\0';
        else         bufferR[0]    = '\0';

        if (strcmp(bufferR, "fin") == 0) break;

        Produit *p = recherche_produit(bufferR);
        if (p == NULL) {
            write(sock, "Produit inconnu, essayez un autre.\n", 35);
            continue;  
        }

        write(sock, "Quantite : ", 11);
        nb = read(sock, bufferR, BUFFER_SIZE);
        if (nb <= 0) break;
        if (nb >= 2) bufferR[nb-2] = '\0';
        else         bufferR[0]    = '\0';
        quantite = atoi(bufferR);

        if (quantite <= 0) {
            write(sock, "Quantite invalide.\n", 19);
            continue;
        }

        float sous_total = p->prix * quantite;
        total_commande += sous_total;

        sprintf(bufferW, "  -> %d x %s = %.2f euros\n", quantite, p->nom, sous_total);
        write(sock, bufferW, strlen(bufferW));

        fprintf(fichier, "%-20s %-10d %.2f euros\n", p->nom, quantite, sous_total);
    }

    sprintf(bufferW, "\nTOTAL COMMANDE : %.2f euros\n", total_commande);
    write(sock, bufferW, strlen(bufferW));

    fprintf(fichier, "TOTAL : %.2f euros\n", total_commande);
    fclose(fichier);
}

int main(void) 
{
    int sock_contact, sock_connectee;
    struct sockaddr_in adresse;
    socklen_t longueur = sizeof(struct sockaddr_in);

    signal(SIGCHLD, SIG_IGN);  

    sock_contact = cree_socket_tcp_ip();
    listen(sock_contact, 5);

    while (1) {
        sock_connectee = accept(sock_contact, (struct sockaddr*)&adresse, &longueur);

        if (fork() == 0) {
            close(sock_contact);
            traite_connection(sock_connectee);
            close(sock_connectee);
            exit(0);
        }
        close(sock_connectee);
    }
    return 0;
}