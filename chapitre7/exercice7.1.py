import os
import signal
import time

pid = os.fork()

if pid == 0:
    # FILS
    while True:
        print("Fils en cours d'exécution...")
        time.sleep(1)

else:
    # PERE
    while True:
        print("\nMenu:")
        print("s: stop")
        print("r: resume")
        print("q: quit")

        choix = input("Choix: ")

        if choix == 's':
            os.kill(pid, signal.SIGSTOP)

        elif choix == 'r':
            os.kill(pid, signal.SIGCONT)

        elif choix == 'q':
            os.kill(pid, signal.SIGKILL)
            break