/*: Créer un client qui se connecte au serveur précédent (localhost:8080), reçoit le message et l'affiche.*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
 #include <arpa/inet.h>
#include <string.h>

int main()
{
    struct sockaddr_in server;

    int client_fd=socket(AF_INET,SOCK_STREAM,0);
    if(client_fd<0)
    {
        fprintf(stderr,"Erreur socket\n");
        return -1;
    
    }
    memset(&server,0,sizeof(struct sockaddr_in));
    server.sin_family=AF_INET;
    server.sin_port=htons(8080);
    inet_pton(AF_inet, "127.0.0.1", &server.sin_addr);
    /*adresse du serveur*/

    /*connection au serveur:*/
    if(connect(client_fd,(struct sockaddr*) &server, sizeof(struct sockaddr_in))<0)
    {
        fprintf(stderr,"Erreur connect\n");
        return -1;
    {
    char buffer[100];
    int n=read(client_fd,buffer,100);
    if(n<0)
    {
        fprintf(stderr,"Erreur read\n");
        return -1;
    }
    buffer[n]='\0';
    printf("Message du serveur: %s\n",buffer);
    close(client_fd);   

    return 0;
}