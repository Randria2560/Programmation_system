import os
import struct
import sys

def main():
    # 1. Saisie du nombre d'entiers (équivalent scanf %d)
    try:
        n = int(input("Combien d'entiers voulez-vous saisir ? "))
    except ValueError:
        print("Erreur : Veuillez entrer un nombre entier.")
        sys.exit(1)

    # 2. Saisie des entiers (remplace la boucle for et malloc)
    # En Python, la gestion mémoire est automatique, pas besoin de malloc()
    tab = []
    for i in range(n):
        try:
            val = int(input(f"Entier {i + 1} : "))
            tab.append(val)
        except ValueError:
            print("Erreur : Entrée invalide.")
            sys.exit(1)

    # 3. Ouverture avec descripteur de fichier [1, 2]
    # Drapeaux : O_WRONLY (écriture seule), O_CREAT (création), O_TRUNC (écraser)
    # Mode : 0o664 (Permissions octales : rw-rw-r--) [3]
    try:
        flags = os.O_WRONLY | os.O_CREAT | os.O_TRUNC
        mode = 0o664 
        fd = os.open("tableau.bin", flags, mode)
    except OSError as e:
        print(f"Erreur lors de la création du fichier: {e}")
        sys.exit(1)

    # 4. Sauvegarde binaire [4, 5]
    # 'struct.pack' transforme la liste d'entiers en bloc d'octets binaires
    # Le format '{n}i' signifie 'n' entiers de type 'int' (standard 4 octets)
    binary_data = struct.pack(f'{n}i', *tab)
    
    # Appel à write() via le descripteur [4, 5]
    os.write(fd, binary_data)

    print("Tableau sauvegardé avec succès dans 'tableau.bin'.")

    # 5. Fermeture du descripteur [6]
    os.close(fd)
    # free(tab) est inutile en Python (Garbage Collector)

if __name__ == "__main__":
    main()