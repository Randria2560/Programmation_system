import threading
import time
import random

TAILLE_TAMPON = 10

tampon = bytearray(TAILLE_TAMPON)
sommet = 0

sem_vide  = threading.Semaphore(TAILLE_TAMPON)
sem_plein = threading.Semaphore(0)
mutex     = threading.Lock()

nb_produits  = 0
nb_consommes = 0

def push(valeur):
    global sommet
    tampon[sommet] = valeur
    sommet += 1

def pop():
    global sommet
    sommet -= 1
    return tampon[sommet]

def producteur(id_):
    global nb_produits

    while True:
        duree = random.randint(1, 3)
        time.sleep(duree)

        octet = random.randint(65, 90)

        sem_vide.acquire()

        with mutex:
            push(octet)
            nb_produits += 1
            print(f"Producteur  {id_:2d} : dépose   '{chr(octet)}'  "
                  f"[tampon : {sommet:2d}/{TAILLE_TAMPON} places]")

        sem_plein.release()

def consommateur(id_):
    global nb_consommes

    while True:
        duree = random.randint(1, 3)
        time.sleep(duree)

        sem_plein.acquire()

        with mutex:
            octet = pop()
            nb_consommes += 1
            print(f"Consommateur {id_:2d} : retire   '{chr(octet)}'  "
                  f"[tampon : {sommet:2d}/{TAILLE_TAMPON} places]")

        sem_vide.release()

def statistiques():
    while True:
        time.sleep(10)
        print(f"\n─── Stats : {nb_produits} produits, {nb_consommes} consommés ───\n")

def main():
    try:
        N = int(input("Nombre de producteurs  (N) : "))
        M = int(input("Nombre de consommateurs (M) : "))
    except ValueError:
        print("Veuillez entrer des entiers valides.", flush=True)
        return

    if N <= 0 or M <= 0:
        print("N et M doivent être > 0")
        return

    print(f"\n=== Démarrage : {N} producteurs, "
          f"{M} consommateurs, tampon {TAILLE_TAMPON} ===\n")

    threads = []

    for i in range(N):
        t = threading.Thread(target=producteur, args=(i + 1,), daemon=True)
        t.start()
        threads.append(t)

    for i in range(M):
        t = threading.Thread(target=consommateur, args=(i + 1,), daemon=True)
        t.start()
        threads.append(t)

    t_stats = threading.Thread(target=statistiques, daemon=True)
    t_stats.start()

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("\nArrêt demandé.")

if __name__ == "__main__":
    main()