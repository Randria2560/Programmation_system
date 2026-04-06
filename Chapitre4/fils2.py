import os
import sys

def main():
    # Récupérer les variables d'environnement (équivalent de getenv)
    env_lecture = os.environ.get("FD_LECTURE")
    env_ecriture = os.environ.get("FD_ECRITURE")

    if env_lecture is None or env_ecriture is None:
        print("Erreur : Variables d'environnement manquantes")
        sys.exit(1)

    # Convertir les chaînes en entiers (équivalent de atoi)
    fd_lire = int(env_lecture)
    fd_ecrire = int(env_ecriture)

    # Fermer le côté écriture (le fils ne s'en sert pas ici)
    os.close(fd_ecrire)

    # Lire le message depuis le pipe
    # os.read(fd, n) lit au maximum n octets et renvoie des bytes
    try:
        message_bytes = os.read(fd_lire, 255)
        
        # Convertir les bytes en chaîne de caractères pour l'affichage
        # On décode et on retire le caractère nul '\0' si présent
        message = message_bytes.decode().strip('\x00')
        
        print(f"Message lu: {message}")
    except OSError as e:
        print(f"Erreur lors de la lecture : {e}")
    finally:
        # Fermer le descripteur de lecture
        os.close(fd_lire)

if __name__ == "__main__":
    main()