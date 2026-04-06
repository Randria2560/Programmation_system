import os
import signal
import time
import sys

# Fonction exécutée lors de la réception du signal (Signal Handler)
def gestionnaire(numero, frame):
    if numero == signal.SIGUSR1:  # Vérification du type de signal [4]
        try:
            # Ouverture et lecture de l'entier dans le fichier [3]
            with open("/tmp/entier.txt", "r") as fp:
                n = fp.read().strip()
                print(f"\n[NOTIFICATION] L'entier enregistré est : {n}")
                sys.exit(0)  # Terminaison propre après lecture
        except FileNotFoundError:
            print("\nErreur : Impossible de lire /tmp/entier.txt")

def main():
    # Affichage du PID pour permettre la communication avec saisit.py [1, 5]
    print(f"Mon PID est : {os.getpid()}")

    # Association de la fonction de capture pour SIGUSR1 [3]
    # Remplace l'utilisation de la structure sigaction en C
    signal.signal(signal.SIGUSR1, gestionnaire)

    # Boucle d'attente utilisant sleep pour économiser les ressources [3, 6]
    try:
        while True:
            print("J'attends que saisit.py m'envoie un signal...")
            time.sleep(10) 
    except KeyboardInterrupt:
        # Permet de quitter proprement avec Ctrl-C si besoin
        pass

if __name__ == "__main__":
    main()