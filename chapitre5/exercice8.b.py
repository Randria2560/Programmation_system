import threading
import time
import random
import sys

NB_ITERATIONS = 10

N = 0
compteur = 0
mutex = threading.Lock()
barriere = threading.Semaphore(0)


def rendez_vous(thread_id, iteration):
    global compteur

    with mutex:
        compteur += 1
        print(f"Thread {thread_id} [iter {iteration}] : arrivé au RDV ({compteur}/{N})")

        if compteur == N:
            print(f"─── Tous arrivés à l'iter {iteration} ! On repart ───\n")
            compteur = 0
            for _ in range(N):
                barriere.release()

    barriere.acquire()


def thread_func(thread_id):
    for i in range(1, NB_ITERATIONS + 1):
        duree = random.randint(1, 5)
        print(f"Thread {thread_id} [iter {i}] : travaille {duree} secondes")
        time.sleep(duree)

        rendez_vous(thread_id, i)

        print(f"Thread {thread_id} [iter {i}] : repart !")


def main():
    global N

    if len(sys.argv) < 2:
        print(f"Usage : {sys.argv[0]} <N>", file=sys.stderr)
        sys.exit(1)

    N = int(sys.argv[1])
    if N <= 0:
        print("N doit être > 0", file=sys.stderr)
        sys.exit(1)

    threads = []
    for i in range(1, N + 1):
        t = threading.Thread(target=thread_func, args=(i,))
        threads.append(t)

    for t in threads:
        t.start()

    for t in threads:
        t.join()


if __name__ == "__main__":
    main()