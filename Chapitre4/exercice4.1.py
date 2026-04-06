import os
import sys

BUFFER_SIZE = 256

def main():
    tube1_r, tube1_w = os.pipe()
    tube2_r, tube2_w = os.pipe()

    fils = os.fork()

    if fils == -1:
        print("fork error", file=sys.stderr)
        sys.exit(1)

    if fils == 0:
        os.close(tube1_r)
        os.close(tube2_w)

        mot = input("Saisissez le mot : ")

        os.write(tube1_w, mot.encode().ljust(BUFFER_SIZE, b'\x00'))
        os.close(tube1_w)

        data = os.read(tube2_r, 4)
        os.close(tube2_r)

        valR = int.from_bytes(data, byteorder='little')

        if valR == 1:
            print(f"Le mot '{mot}' existe dans le fichier.")
        else:
            print(f"Le mot '{mot}' n'existe pas dans le fichier.")

    else:
        os.close(tube1_w)
        os.close(tube2_r)

        data = os.read(tube1_r, BUFFER_SIZE)
        os.close(tube1_r)

        mot = data.rstrip(b'\x00').decode()

        try:
            with open("./fichier.txt", "r") as fichier:
                valW = 0
                for ligne in fichier:
                    if mot in ligne:
                        valW = 1
                        break
        except OSError as e:
            print(f"fopen error: {e}", file=sys.stderr)
            sys.exit(1)

        os.write(tube2_w, valW.to_bytes(4, byteorder='little'))
        os.close(tube2_w)

        os.wait()

if __name__ == "__main__":
    main()