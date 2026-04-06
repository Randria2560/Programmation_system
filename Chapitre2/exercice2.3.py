import os
import sys

def main():
    fils = [None] * 6

    for i in range(1, 6):
        fils[i] = os.fork()

        if fils[i] == -1:
            print(f"Error creating process: {os.strerror(os.errno)}", file=sys.stderr)
            sys.exit(1)

        if fils[i] == 0:
            print(f"I am in fils[{i}] : me={os.getpid()}, father={os.getppid()}")
            break

if __name__ == "__main__":
    main()
