import sys

def main():
    sum = 0
    if len(sys.argv) != 3:
        print("Pas d'argument correct", file=sys.stderr)
        sys.exit(1)
    else:
        for j in range(len(sys.argv) - 1):
            sum += int(sys.argv[j + 1])
        print(f"La somme est: {sum}")

if __name__ == "__main__":
    main()