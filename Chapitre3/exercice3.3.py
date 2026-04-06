import os
import sys

def main():
    fichier = input("Entrer le nom du repertoire: ")
    try:
        os.execl("/usr/bin/vim", "vim", fichier)
    except OSError as e:
        print(f"Erreur execv: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()