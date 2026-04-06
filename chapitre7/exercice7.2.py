import os
import sys
import signal

def main():
    # Vérification des arguments (équivalent à argc/argv [1])
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv} pid_de_affiche", file=sys.stderr)
        sys.exit(1)

    # Récupération du PID et conversion (équivalent à atoi [2])
    try:
        pid_dest = int(sys.argv[3])
    except ValueError:
        print("Erreur: Le PID doit être un entier.", file=sys.stderr)
        sys.exit(1)

    # Saisie de l'entier
    try:
        n = int(input("Veuillez saisir un entier : "))
    except ValueError:
        print("Erreur: La saisie doit être un nombre entier.", file=sys.stderr)
        sys.exit(1)

    # Enregistrement dans le fichier /tmp/entier.txt
    try:
        with open("/tmp/entier.txt", "w") as fp:
            fp.write(str(n))
    except IOError as e:
        print(f"Erreur lors de l'ouverture du fichier: {e}", file=sys.stderr)
        sys.exit(1)

    # Envoi du signal SIGUSR1 au PID cible (équivalent à kill [4, 5])
    try:
        os.kill(pid_dest, signal.SIGUSR1)
        print(f"Signal SIGUSR1 envoyé avec succès au PID {pid_dest}.")
    except ProcessLookupError:
        print(f"Erreur: Aucun processus ne possède le PID {pid_dest}.", file=sys.stderr)
    except PermissionError:
        print(f"Erreur: Permission refusée pour envoyer un signal à {pid_dest}.", file=sys.stderr)

if __name__ == "__main__":
    main()