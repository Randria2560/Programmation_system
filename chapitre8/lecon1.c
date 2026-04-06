/*Création d'un socket*/
/*socket(AF_INET, SOCK_STREAM, 0) : Crée la socket en spécifiant IPv4 (AF_INET) et le protocole fiable TCP (SOCK_STREAM)
memset : Vide la structure mémoire pour éviter des erreurs avec des données résiduelles
htons(0) : Demande au système de choisir lui-même un port libre
INADDR_ANY : Permet au serveur d'écouter sur toutes les cartes réseau (Wi-Fi, Ethernet, localhost) de l'ordinateur
bind() : Enregistre officiellement cette configuration auprès du système*/
int cree_socket_tcp_ip() {
    int sock;
    struct sockaddr_in adresse;

    // 1. Création de l'identifiant de la socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Erreur socket\n");
        return -1;
    }

    // 2. Initialisation de la structure d'adresse à zéro
    memset(&adresse, 0, sizeof(struct sockaddr_in));

    // 3. Configuration de la famille d'adresses (IPv4)
    adresse.sin_family = AF_INET;

    // 4. Attribution d'un port disponible aléatoire (0)
    adresse.sin_port = htons(0);

    // 5. Acceptation de toutes les adresses IP de la machine
    
    adresse.sin_addr.s_addr = htons(INADDR_ANY);

    // 6. Liaison (attachement) de la socket à l'adresse et au port
    if (bind(sock, (struct sockaddr*) &adresse, sizeof(struct sockaddr_in)) < 0) {
        close(sock);
        fprintf(stderr, "Erreur bind\n");
        return -1;
    }
    return sock;

    /*autre expemple:
    // connexion sur le port 33016 
    // adresse.sin_port = htons(33016);
    // depuis localhost seulement :
    // inet_aton("127.0.0.1", &adresse.sin_addr);
    if(bind(sock,(struct sockaddr*) &adresse,
    sizeof(struct sockaddr_in)) < 0)
    {
        close(sock);
        fprintf(stderr, "Erreur bind\(\backslash\)n");
        return -1;
    }
    */
}


//==================================================
/*2. Affichage de l'adresse de la Socket (Page 65)
Cette fonction permet de savoir quel port a été attribué au programme.*/
int affiche_adresse_socket(int sock) {
    struct sockaddr_in adresse;
    socklen_t longueur = sizeof(struct sockaddr_in);

    // Récupère les informations actuelles de la socket
    if (getsockname(sock, (struct sockaddr*)&adresse, &longueur) < 0) {
        fprintf(stderr, "Erreur getsockname\n");
        return -1;
    }

    // Affiche l'IP et le Port convertis en format lisible
    printf("IP = %s, Port = %u\n", inet_ntoa(adresse.sin_addr), ntohs(adresse.sin_port));
    return 0;
}
    /*getsockname : Interroge le système pour connaître l'adresse et le port réellement utilisés par la socket
    .
    inet_ntoa : Convertit l'adresse IP binaire en une chaîne de caractères (ex: "127.0.0.1")
    .
    ntohs : Convertit le numéro de port (format réseau) en entier lisible (format hôte)*/



/*Servueur TCP/IP*/
/*C'est le "cœur" du programme serveur qui attend les clients.*/
int main(void) {
    int sock_contact, sock_connectee;
    // ... initialisations ...

    // 1. Initialise la socket de contact
    sock_contact = cree_socket_tcp_ip();
    
    // 2. Met la socket en mode écoute (file d'attente de 5 clients max)
    listen(sock_contact, 5);

    while (1) {
        // 3. Attend une connexion (bloquant) et crée une nouvelle socket dédiée
        sock_connectee = accept(sock_contact, (struct sockaddr*)&adresse, &longueur);
        
        // 4. Crée un fils pour gérer ce client spécifique
        if (fork() == 0) {
            close(sock_contact); // Le fils n'a plus besoin de la socket d'écoute
            traite_connection(sock_connectee); // Dialogue avec le client
            exit(0); // Le fils se termine après le dialogue
        } else {
            close(sock_connectee); // Le père ferme la socket dédiée et retourne attendre
        }
    }
}
    /*listen() : Indique au noyau que le programme est prêt à recevoir des appels
    .
    accept() : Suspend le programme jusqu'à ce qu'un client se connecte. Elle retourne une nouvelle socket (sock_connectee) pour ne pas bloquer l'entrée principale
    .
    fork() : Permet de traiter plusieurs clients en même temps en créant un processus par client*/



/*Dialogue avec le Client (Page 67)
Une fois connecté, on utilise les fonctions de lecture/écriture classiques.*/
void traite_connection(int sock) {
    // ... initialisations ...
    // Récupère l'identité du client (IP/Port)
    getpeername(sock, (struct sockaddr*) &adresse, &longueur);
    
    // Envoie des messages au client
    write(sock, "Votre adresse : ", 16);
    write(sock, bufferW, strlen(bufferW)+1);
    
    // Lit ce que l'utilisateur tape sur son clavier distant
    nb = read(sock, bufferR, BUFFER_SIZE);
    printf("L'utilisateur distant a tapé : %s\n", bufferR);
}
    /*getpeername : Identifie la machine distante
    write() / read() : Fonctionnent exactement comme pour un tube ou un fichier*/

/*Client TCP/IP : Connexion (Page 69)
Ce code permet à un programme de contacter un serveur distant.*/
int cree_socket_tcp_client(int argc, char** argv) {
    struct sockaddr_in adresse;
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    // Configure l'adresse du serveur cible
    adresse.sin_family = AF_INET;
    adresse.sin_port = htons(atoi(argv[12])); // Port passé en argument
    inet_aton(argv[13], &adresse.sin_addr);    // IP passée en argument

    // 1. Tente d'établir la connexion
    if (connect(sock, (struct sockaddr*) &adresse, sizeof(struct sockaddr_in)) < 0) {
        return -1;
    }
    return sock;
}

    /*inet_aton : Convertit l'IP écrite par l'utilisateur en adresse binaire exploitable
    connect() : Déclenche la demande de connexion vers le serveur*/


/*6. Boucle de Dialogue du Client (Page 70)*/
while(1) {
    // 1. Lit ce que le serveur envoie et l'affiche sur l'écran local
    if (read(sock, buffer, BUFFER_SIZE) == 0) break;
    puts(buffer);

    // 2. Lit une phrase au clavier local et l'envoie au serveur
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) break;
    write(sock, buffer, BUFFER_SIZE);
}
    /*Le client alterne entre écouter le réseau (read) et envoyer ses saisies clavier (write*/