void traite_connection(int sock) {
    struct sockaddr_in adresse;
    socklen_t longueur;
    char bufferR[BUFFER_SIZE];
    char bufferW[BUFFER_SIZE];
    int nb;

    longueur = sizeof(struct sockaddr_in);
    // 1. Récupération de l'identité du client distant
    if (getpeername(sock, (struct sockaddr*) &adresse, &longueur) < 0) {
        fprintf(stderr, "Erreur getpeername\n");
        return;
    }

    // 2. Formatage de l'adresse IP et du port du client
    sprintf(bufferW, "IP = %s, Port = %u\n",
            inet_ntoa(adresse.sin_addr), ntohs(adresse.sin_port));

    // 3. Affichage des informations sur la console du serveur
    printf("Connexion : locale (sock_connectee) ");
    affiche_adresse_socket(sock);
    printf(" Machine distante : %s", bufferW);

    // 4. Envoi de messages de bienvenue au client via la socket
    write(sock, "Votre adresse : ", 16);
    write(sock, bufferW, strlen(bufferW)+1);
    strcpy(bufferW, "Veuillez entrer une phrase : ");
    write(sock, bufferW, strlen(bufferW)+1);

    // 5. Lecture de la réponse du client
    nb = read(sock, bufferR, BUFFER_SIZE);
    bufferR[nb-2] = '\0'; // Nettoyage des caractères de fin de ligne
    printf("L'utilisateur distant a tapé : %s\n", bufferR);

    // 6. Confirmation et attente de fin
    sprintf(bufferW, "Vous avez tapé : %s\n", bufferR);
    strcat(bufferW, "Appuyez sur entree pour terminer\n");
    write(sock, bufferW, strlen(bufferW)+1);
    read(sock, bufferR, BUFFER_SIZE);
}
