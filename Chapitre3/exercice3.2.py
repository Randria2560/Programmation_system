import os
import subprocess
import sys

def main():
    home = os.environ.get("HOME")
    if home is None:
        return 1

    nouveau_repertoire = os.path.join(home, "nouveau_repertoire")

    subprocess.run(["mkdir", nouveau_repertoire])

    os.execv("/usr/bin/cp", ["cp", "-r", ".", nouveau_repertoire])

    print("Erreur execv", file=sys.stderr)
    return 1

if __name__ == "__main__":
    sys.exit(main())