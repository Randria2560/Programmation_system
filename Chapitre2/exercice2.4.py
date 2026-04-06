import os
import sys

i = 0

def main():
    global i
    for i in range(1, 6):
        fils = os.fork()

        if fils == -1:
            print("Erreur lors de la création d'un des processus", file=sys.stderr)
            sys.exit(1)

        if fils == 0:  # I am in the child
            print(f"Je suis dans le fils{i}: moi={os.getpid()},père={os.getppid()}")
            break

if __name__ == "__main__":
    main()