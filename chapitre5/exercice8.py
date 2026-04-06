import threading
import time
import random

NB_ITERATIONS = 10

sem_t1 = threading.Semaphore(0)
sem_t2 = threading.Semaphore(0)

def thread_T1():
    for i in range(1, NB_ITERATIONS + 1):
        print(f"T1 [iter {i}] : début du traitement")
        time.sleep(2)
        print(f"T1 [iter {i}] : traitement terminé, j'attends T2")

        sem_t1.release()
        sem_t2.acquire()

        print(f"T1 [iter {i}] : rendez-vous OK, on repart !\n")

def thread_T2():
    for i in range(1, NB_ITERATIONS + 1):
        duree = 4 + random.randint(0, 5)
        print(f"T2 [iter {i}] : début du traitement ({duree} secondes)")
        time.sleep(duree)
        print(f"T2 [iter {i}] : traitement terminé, j'attends T1")

        sem_t2.release()
        sem_t1.acquire()

        print(f"T2 [iter {i}] : rendez-vous OK, on repart !\n")

if __name__ == "__main__":
    t1 = threading.Thread(target=thread_T1)
    t2 = threading.Thread(target=thread_T2)

    t1.start()
    t2.start()

    t1.join()
    t2.join()