import os
import sys

def main():

    while True:
        # ① Saisir le nom du fichier
        nom_fichier = input("\nEntrez le nom du fichier à ouvrir "
                           "(ou 'q' pour quitter) : ").strip()

        # ② Condition de sortie
        if nom_fichier == "q":
            break

        # ③ Vérifier que le nom n'est pas vide
        if not nom_fichier:
            print("Nom vide, veuillez recommencer.")
            continue

        # ④ Créer un fils pour lancer vim
        pid = os.fork()

        if pid == -1:
            print("Erreur fork")
            sys.exit(1)

        if pid == 0:
            # ── FILS : lancer vim
            os.execl("/usr/bin/vim", "vim", nom_fichier)

            # Atteint seulement si execl échoue
            print("Erreur lors de l'ouverture de vim")
            sys.exit(1)

        else:
            # ── PÈRE : attendre la fermeture de vim
            pid_termine, status = os.waitpid(pid, 0)

            if os.WIFEXITED(status) and os.WEXITSTATUS(status) == 0:
                print("Fermeture de vim détectée. "
                      "Prêt pour un nouveau fichier.")
            else:
                print("vim s'est terminé avec une erreur.")

    print("\nFin du programme.")

if __name__ == "__main__":
    main()