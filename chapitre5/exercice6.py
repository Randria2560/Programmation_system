import threading
import time
import random

N = 100
tab = [0.0] * N
mon_mutex = threading.Lock()

def remplace():
    while True:
        with mon_mutex:
            for i in range(1, 99):
                tab[i] = (tab[i - 1] + tab[i] + tab[i + 1]) / 3.0
        nbsec = random.randint(1, 3)
        time.sleep(nbsec)

def affiche():
    while True:
        with mon_mutex:
            print("\n--- État du tableau ---")
            for i in range(N):
                print(f"Valeur [{i}]: {tab[i]:.2f}  ", end="")
                if i % 5 == 4:
                    print()
        time.sleep(4)

def main():
    random.seed(time.time())

    tab[0] = 0.0
    tab[99] = 0.0
    for i in range(1, 99):
        tab[i] = float(random.randint(0, 99))

    t1 = threading.Thread(target=remplace, daemon=True)
    t2 = threading.Thread(target=affiche, daemon=True)

    t1.start()
    t2.start()

    t1.join()
    t2.join()

if __name__ == "__main__":
    main()