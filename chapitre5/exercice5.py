import threading
import time
import random
import sys

compteur = 0
mon_mutex = threading.Lock()

def incrementation(limite):
    global compteur
    while True:
        with mon_mutex:
            if compteur >= limite:
                break
            compteur += 1
        time.sleep(random.randint(1, 5))

def affiche(limite):
    global compteur
    while True:
        with mon_mutex:
            if compteur >= limite:
                break
            print(f"Valeur actuelle du compteur : {compteur}")
        time.sleep(2)

def main():
    global compteur
    if len(sys.argv) < 2:
        return 1
    limite = int(sys.argv[1])

    t1 = threading.Thread(target=incrementation, args=(limite,))
    t2 = threading.Thread(target=affiche, args=(limite,))

    t1.start()
    t2.start()

    t1.join()
    t2.join()

    print(f"Fin du programme. Compteur final : {compteur}")
    return 0

if __name__ == "__main__":
    sys.exit(main())