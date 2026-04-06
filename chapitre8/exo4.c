/*
serveur multi-clients*/
void handle_client(int client_fd)) 
{

    // ... traitement de la connexion avec le client ...
}
while(1)
{
    int client_fd=accept(server_fd,NULL,NULL);
    if(client_fd<0)
    {
        fprintf(stderr,"Erreur accept\n");
        return -1;
    }
    pid=fork();
    if(pid==0) /*Processus fils*/
    {
        close(server_fd); /*Le fils n'écoute pas*/
        handle_client(client_fd);
        close(client_fd);
        exit(0);
    }
    else /*Processus parent*/
    {
        close(client_fd); /*Le parent ne parle pas*/
    }
}