import os
import sys

#pourquoi je n'arrive pas à voire le variable d'environnement

def main():
    # Création du tube (pipe)
    try:
        tube = os.pipe() # tube[0] est lecture, tube[1] est écriture
    except OSError:
        print("Erreur : pipe")
        sys.exit(1)

    # Création du processus fils
    try:
        fils = os.fork()
    except OSError:
        print("Erreur : fork")
        sys.exit(1)

    # Préparation des chaînes pour l'environnement (commun aux deux avant l'exec)
    fd_lecture = str(tube[0])
    fd_ecriture = str(tube[1])

    # Équivalent de setenv(name, value, overwrite)
    os.environ["FD_LECTURE"] = fd_lecture
    os.environ["FD_ECRITURE"] = fd_ecriture

    if fils == 0:
        
        # Le fils n'a pas besoin d'arguments dans args car il lira l'environnement
        args = ["./fils"]

        try:
            # os.execv remplace le processus actuel par le binaire "./fils"
            os.execv("./fils", args)
        except OSError:
            print("Erreur : execv")
            sys.exit(1)

    else:
        
        # Le père ne lit pas, on ferme le côté lecture du tube
        os.close(tube[0])

        message = "Bonjour tout le monde"
        
        # En Python, on écrit des bytes. On ajoute '\0' pour être identique au C
        os.write(tube[1], (message + '\0').encode())
        
        # Fermer le côté écriture
        os.close(tube[1])

        # Attendre la fin du fils
        os.wait()

if __name__ == "__main__":
    main()