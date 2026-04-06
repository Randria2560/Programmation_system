import os
import sys

pid = os.fork()

if pid == -1:
    print("Erreur lors de la création de processus", file=sys.stderr)
    sys.exit(1)
elif pid == 0:
    print(f"Je suis dans le fils: moi={os.getpid()},père={os.getppid()}")
else:
    print(f"Je suis le père: fils:{pid},moi:{os.getpid()},mon_père:{os.getppid()}")