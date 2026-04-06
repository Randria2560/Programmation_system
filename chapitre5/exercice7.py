import threading
import time
import random
import math

N = 100
compte = 0
u = 0.0
mutex = threading.Lock()
nbsec = 0


def thread1():
    global compte, u, nbsec
    while True:
        with mutex:
            compte += 1
            u = (1.0 / 4.0) * math.pow(u - 1, 2)
            print(f"La valeur de u dans 1: {u}")
            nbsec = random.randint(1, 5)
        time.sleep(nbsec)


def thread2():
    global compte, u, nbsec
    while True:
        with mutex:
            compte += 1
            u = (1.0 / 6.0) * math.pow(u - 2, 2)
            print(f"La valeur de u dans 2: {u}")
            nbsec = random.randint(1, 5)
        time.sleep(nbsec)


if __name__ == "__main__":
    u = 1.0

    t1 = threading.Thread(target=thread1)
    t2 = threading.Thread(target=thread2)

    t1.start()
    t2.start()

    t1.join()
    t2.join()