import os
import sys

def main():
    # Création du tube (pipe)
    try:
        r, w = os.pipe()
    except OSError:
        print("Erreur : pipe")
        sys.exit(1)

    # Création du processus fils
    try:
        pid = os.fork()
    except OSError:
        print("Erreur : fork")
        sys.exit(1)

    if pid == 0:
        # --- PROCESSUS FILS ---
        
        # Convertir les descripteurs de fichiers en chaînes pour les passer en arguments
        lecture = str(r)
        ecriture = str(w)

        # Préparation des arguments pour execv
        # En Python, le premier argument de la liste args doit être le nom du programme
        args = ["./fils", lecture, ecriture]

        try:
            # os.execv remplace le processus actuel par le programme spécifié
            os.execv("./fils", args)
        except OSError:
            print("Erreur : execv")
            sys.exit(1)

    else:
        # --- PROCESSUS PÈRE ---
        
        # Le père ne lit pas, on ferme le côté lecture
        os.close(r)

        message = "Bonjour tout le monde: ceci est le message"
        
        # En Python, on doit encoder la chaîne en bytes avant de l'écrire dans un pipe
        # +1 pour simuler le '\0' de fin de chaîne en C (facultatif en pur Python mais fidèle au C)
        os.write(w, (message + '\0').encode())

        # Fermer le côté écriture après l'envoi
        os.close(w)

        # Attendre la fin du fils
        os.wait()

if __name__ == "__main__":
    main()