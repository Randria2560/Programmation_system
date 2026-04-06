import os
import sys

def main():
    # Vérification du nombre d'arguments [1]
    # sys.argv est le nom du script, donc len(sys.argv) doit être >= 2
    if len(sys.argv) < 2:
        print(f"Usage : {sys.argv} répertoire1 répertoire2 ...", file=sys.stderr)
        sys.exit(1)

    # On boucle sur chaque répertoire passé en argument [1, 2]
    for i in range(1, len(sys.argv)):
        repertoire = sys.argv[i]
        
        try:
            # os.listdir remplace opendir/readdir et retourne la liste des noms [2, 3]
            entrees = os.listdir(repertoire)
        except OSError:
            print(f"Erreur d'ouverture du répertoire : {repertoire}")
            continue

        print(f"\n--- Analyse du répertoire : {repertoire} ---")
        
        for nom_entree in entrees:
            # Construction du chemin complet (remplace sprintf) [4]
            chemin_complet = os.path.join(repertoire, nom_entree)

            try:
                # os.stat remplace la fonction stat() du C [5, 6]
                st = os.stat(chemin_complet)

                # Vérification : fichier ordinaire (S_ISREG) et taille > 1 Mo [7-9]
                # 1 Mo = 1024 * 1024 = 1 048 576 octets
                if os.path.isfile(chemin_complet) and st.st_size > 1048576:
                    # st.st_uid correspond à l'UID du propriétaire [6, 10, 11]
                    print(f"Nom : {nom_entree} | Taille : {st.st_size} octets | UID : {st.st_uid}")
            except OSError:
                # Si le fichier est inaccessible pendant l'analyse
                continue

if __name__ == "__main__":
    main()