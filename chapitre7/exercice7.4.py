import sys

def main():
    # 1. Saisie de la taille et "allocation"
    # En Python, la gestion mémoire est automatique, pas de malloc/free [1]
    try:
        n = int(input("Entrez la taille du tableau (n) : "))
    except ValueError:
        print("Erreur : n doit être un entier.")
        return

    # Création d'une liste (équivalent du tableau alloué dynamiquement) [1]
    tab =  * n

    # 2. Remplissage du tableau
    for j in range(n):
        try:
            tab[j] = int(input(f"tab[{j}] = "))
        except ValueError:
            print("Entrée invalide, valeur mise à 0.")

    # 3, 4 & 5. Gestion de l'erreur et point de retour
    # Le bloc try/except remplace ici sigaction, sigsetjmp et siglongjmp [3, 4]
    while True:
        try:
            i_input = input("\nEntrez l'indice i de l'élément à afficher : ")
            if not i_input: 
                break
            i = int(i_input)

            # En C, cette ligne génère un SIGSEGV si i est invalide [1, 2]
            # En Python, cela lève un IndexError
            print(f"Valeur de tab[{i}] = {tab[i]}")
            
            # Si l'affichage réussit, on sort de la boucle (fin du programme)
            break 

        except IndexError:
            # Équivalent du gestionnaire_segv [4, 5]
            print("\n[ERREUR] Erreur de segmentation détectée (i est hors limites) !")
            # La boucle while recommence, simulant le retour au point de sauvegarde
        except ValueError:
            print("Veuillez entrer un nombre entier pour l'indice.")

    print("Fin du programme sans erreur.")

if __name__ == "__main__":
    main()