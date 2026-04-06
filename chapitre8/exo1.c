#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

/*Créer un serveur qui écoute sur le port 8080, accepte une connexion, envoie "Bonjour!" au client, et se ferme.*/

int create_sock()
{
    int sock=socket(AF_INET,SOCK_STREAM,0);
    return sock;
}

//simple server TCP/IP
int main() {
    int server_fd=create_sock();
    //configuration addresse
    struct sockaddr_in address;
    memset(&address,0,sizeof(struct sockaddr_in));
    address.sin_family=AF_INET;
    address.sin_port=htons(8080);
    address.sin_addr.s_addr=htonl(INADDR_ANY);

    //lier le socket avec bind
    if(bind(server_fd,(struct sockaddr*) &address, sizeof(struct sockaddr_in))<0)
    {
        fprintf(stderr,"Erreur bind\n");
        return -1;
    }

    //mettre le socket en écoute
    listen(server_fd, 5);
    printf("Serveur en écoute sur le port 8080...\n");
    while(1)
    {
        //accepter une connexion
        int client_fd=accept(server_fd, NULL, NULL);
        if(client_fd<0)
        {
            fprintf(stderr,"Erreur accept\n");
            return -1;
        }
        //envoyer "Bonjour!" au client
        write(client_fd,"Bonjour!",9);
        //fermer la connexion avec le client
        close(client_fd);
    }

    //ou bien
    send(client_fd,"Bonjour!",9,0);
    //afaka alana le boucle d'écoute
    close(client_fd);
    close(server_fd);
    
    return 0;
}