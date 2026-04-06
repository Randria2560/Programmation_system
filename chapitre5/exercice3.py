import threading
import random
import time

class TypeTableau:
    def __init__(self):
        self.n = 0
        self.t = []
        self.entier = 0

result = None

def generation(tab):
    print("\n Génération du tableau...")
    tab.t = [random.randint(0, 99) for _ in range(tab.n)]
    time.sleep(2)
    print("[Thread 1] Génération terminée.")

def t_recherche(tab):
    global result
    trouve = 0
    for i in range(tab.n):
        if tab.t[i] == tab.entier:
            trouve = 1
            break
    result = trouve

def main():
    global result
    data = TypeTableau()

    nb_elements = int(input("Combien d'éléments pour le tableau ? "))
    data.n = nb_elements

    t1 = threading.Thread(target=generation, args=(data,))
    t1.start()
    t1.join()

    data.entier = int(input("Pendant ce temps, entrez la valeur x à rechercher : "))

    recherche = threading.Thread(target=t_recherche, args=(data,))
    recherche.start()
    recherche.join()

    if result == 1:
        print(f"L'entier {data.entier} a été trouvé dans le tableau.")
    else:
        print(f"L'entier {data.entier} n'est pas dans le tableau.")

if __name__ == "__main__":
    main()