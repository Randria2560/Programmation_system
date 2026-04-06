import os
import sys

def main():
    fils1 = os.fork()
    if fils1 == -1:
        print("Erreur lors de la création d'un des processus", file=sys.stderr)
        sys.exit(1)

    if fils1 == 0:
        print(f"Je suis dans le fils1 : moi={os.getpid()},père={os.getppid()}")
    else:
        fils2 = os.fork()
        if fils2 == 0:
            print(f"Je suis dans le fils2 : moi={os.getpid()},père={os.getppid()}")
        else:
            print(f"Je suis le père: fils1:{fils1},fils2={fils2},moi:{os.getpid()},mon_père:{os.getppid()}")

if __name__ == "__main__":
    main()