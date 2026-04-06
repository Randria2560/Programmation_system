import os
import struct
import sys

def main():
    nom_fichier = "tableau.bin"

    # 1. Obtenir les informations sur le fichier via l'appel système stat [3, 4]
    try:
        st = os.stat(nom_fichier)
    except OSError:
        print("Erreur : impossible de lire les infos du fichier", file=sys.stderr)
        sys.exit(1)

    taille_totale = st.st_size  # Taille en octets [5]
    # En C, un int fait généralement 4 octets. On calcule le nombre d'éléments [6]
    nb_elements = taille_totale // 4 

    print(f"Taille du fichier détectée : {taille_totale} octets ({nb_elements} entiers)")

    # 2. Ouverture du fichier en lecture seule (O_RDONLY) [7]
    try:
        fd = os.open(nom_fichier, os.O_RDONLY)
    except OSError as e:
        print(f"Erreur lors de l'ouverture du fichier: {e}", file=sys.stderr)
        sys.exit(1)

    # 3. Lecture du bloc de données binaire [8]
    # Python lit les octets directement dans un objet 'bytes'
    try:
        donnees_binaires = os.read(fd, taille_totale)
    except OSError as e:
        print(f"Erreur lors de la lecture des données: {e}", file=sys.stderr)
        donnees_binaires = b""

    # 4. Conversion des octets en tableau d'entiers (Unpacking)
    # Contrairement au C qui utilise malloc et un cast (int *), 
    # Python utilise struct.unpack pour interpréter les octets [9]
    if donnees_binaires:
        # Le format '{nb_elements}i' signifie : interpréter comme nb_elements entiers
        tab = struct.unpack(f'{nb_elements}i', donnees_binaires)

        # Affichage pour vérification
        print("Contenu du tableau chargé :")
        for val in tab:
            print(val, end=" ")
        print()

    # 5. Nettoyage [2]
    os.close(fd)
    # free(tab) est inutile car Python gère la mémoire automatiquement

if __name__ == "__main__":
    main()