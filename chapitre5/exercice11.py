import threading
import time
import random

TAILLE_BDD = 15

bdd = bytearray(TAILLE_BDD)

mutex_lecteurs = threading.Lock()
mutex_bdd = threading.Lock()
mutex_stats = threading.Lock()

nb_lecteurs = 0
nb_lectures = 0
nb_ecritures = 0


def afficher_bdd():
    print("  BDD : [" + "".join(f"{b:3d}" for b in bdd) + " ]")


def lecteur(id):
    global nb_lecteurs, nb_lectures

    while True:
        time.sleep(1 + random.randint(0, 2))

        with mutex_lecteurs:
            nb_lecteurs += 1
            if nb_lecteurs == 1:
                mutex_bdd.acquire()

        index = random.randint(0, TAILLE_BDD - 1)
        valeur = bdd[index]

        with mutex_stats:
            nb_lectures += 1

        print(f"Lecteur  {id:2d} : lit    bdd[{index:2d}] = {valeur:3d}  ({nb_lecteurs} lecteurs actifs)")

        with mutex_lecteurs:
            nb_lecteurs -= 1
            if nb_lecteurs == 0:
                mutex_bdd.release()


def redacteur(id):
    global nb_ecritures

    while True:
        time.sleep(1 + random.randint(0, 9))

        mutex_bdd.acquire()

        index = random.randint(0, TAILLE_BDD - 1)
        avant = bdd[index]
        bdd[index] = random.randint(0, 255)

        with mutex_stats:
            nb_ecritures += 1

        print(f"Rédacteur {id:2d} : écrit  bdd[{index:2d}] : {avant:3d} → {bdd[index]:3d}")
        afficher_bdd()

        mutex_bdd.release()


def statistiques():
    while True:
        time.sleep(15)

        with mutex_stats:
            print(f"\nStats : {nb_lectures} lectures, {nb_ecritures} écritures")
            print("════════════════════════════════\n")


def main():
    try:
        N = int(input("Nombre de lecteurs   (N) : "))
        M = int(input("Nombre de rédacteurs (M) : "))
    except ValueError:
        print("Veuillez entrer des entiers valides.", flush=True)
        return

    if N <= 0 or M <= 0:
        print("N et M doivent être > 0")
        return

    print(f"\n=== Démarrage : {N} lecteurs, {M} rédacteurs, BDD {TAILLE_BDD} octets ===\n")
    afficher_bdd()
    print()

    threads = []

    for i in range(N):
        t = threading.Thread(target=lecteur, args=(i + 1,), daemon=True)
        threads.append(t)
        t.start()

    for i in range(M):
        t = threading.Thread(target=redacteur, args=(i + 1,), daemon=True)
        threads.append(t)
        t.start()

    t_stats = threading.Thread(target=statistiques, daemon=True)
    t_stats.start()

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("\nArrêt du programme.")


if __name__ == "__main__":
    main()