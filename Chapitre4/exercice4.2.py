import os
import sys

def main():
    # Taille du tampon (non strictement nécessaire en Python mais gardé pour la cohérence)
    BUFFER_SIZE = 256

    # Création des pipes (tube1: fils -> père, tube2: père -> fils)
    try:
        r1, w1 = os.pipe()
        r2, w2 = os.pipe()
    except OSError:
        print("Erreur lors de la création des pipes")
        sys.exit(1)

    try:
        pid = os.fork()
    except OSError:
        print("Erreur lors du fork")
        sys.exit(1)

    if pid == 0:  # --- PROCESSUS FILS ---
        os.close(r1)  # Ferme lecture tube1
        os.close(w2)  # Ferme écriture tube2

        # Sauvegarder le vrai stdout (terminal)
        stdout_terminal = os.dup(sys.stdout.fileno())

        # Saisie du mot
        # On utilise sys.stderr ou on écrit directement pour éviter les buffers
        sys.stdout.write("Saisissez le mot : ")
        sys.stdout.flush()
        mot = sys.stdin.readline().strip().split()[0] # Équivalent scanf %s

        # Rediriger stdout vers le tube1
        os.dup2(w1, sys.stdout.fileno())
        os.close(w1)

        # Envoyer le mot (le print va dans le tube maintenant)
        print(mot, end='')
        sys.stdout.flush()
        
        # Fermer stdout pour signaler EOF au père
        os.close(sys.stdout.fileno())

        # Restauration de stdout vers le terminal
        os.dup2(stdout_terminal, sys.stdout.fileno())
        os.close(stdout_terminal)

        # Lire le résultat du père depuis tube2
        # os.read renvoie des bytes, on utilise l'unpacking pour l'entier
        import struct
        data = os.read(r2, 4) # sizeof(int) est généralement 4
        os.close(r2)

        if data:
            valR = struct.unpack('i', data)[0]
            if valR == 1:
                print(f"Le mot '{mot}' existe dans le fichier.")
            else:
                print(f"Le mot '{mot}' n'existe pas dans le fichier.")

    else:  # --- PROCESSUS PÈRE ---
        os.close(w1)  # Ferme écriture tube1
        os.close(r2)  # Ferme lecture tube2

        # Rediriger stdin pour lire depuis le tube1
        os.dup2(r1, sys.stdin.fileno())
        os.close(r1)

        # Recevoir le mot du fils (via stdin redirigé)
        mot = sys.stdin.read().strip()

        valW = 0
        try:
            with open("./fichier.txt", "r") as fichier:
                for ligne in fichier:
                    if mot in ligne:
                        valW = 1
                        break
        except FileNotFoundError:
            print("Erreur : fichier.txt introuvable")
            sys.exit(1)

        # Envoyer le résultat au fils via tube2
        import struct
        os.write(w2, struct.pack('i', valW))
        os.close(w2)

        # Attendre la fin du fils
        os.wait()

if __name__ == "__main__":
    main()