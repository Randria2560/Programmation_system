import threading
import semaphore as sem_module
import time
import random

NB_OCTETS = 10

boite = 0

sem_vide = threading.Semaphore(1)
sem_plein = threading.Semaphore(0)


def emetteur():
    global boite

    for i in range(1, NB_OCTETS + 1):
        duree = random.randint(1, 3)
        time.sleep(duree)

        sem_vide.acquire()

        boite = 65 + i
        print(f"Émetteur  [{i:2d}] : dépose   '{chr(boite)}'  (attend {duree}s)")

        sem_plein.release()


def recepteur():
    global boite

    for i in range(1, NB_OCTETS + 1):
        duree = random.randint(1, 3)
        time.sleep(duree)

        sem_plein.acquire()

        lu = boite
        print(f"Récepteur [{i:2d}] : lit      '{chr(lu)}'  (attend {duree}s)")

        sem_vide.release()


def main():
    print("=== Début de la communication ===\n")

    t_emet = threading.Thread(target=emetteur)
    t_recep = threading.Thread(target=recepteur)

    t_emet.start()
    t_recep.start()

    t_emet.join()
    t_recep.join()

    print("\n=== Communication terminée ===")


if __name__ == "__main__":
    main()