import threading

def ma_fonction(tab):
    for value in tab:
        print(value, end=" ")
    print()

def main():
    valeurs = [10, 20, 30, 40]

    t1 = threading.Thread(target=ma_fonction, args=(valeurs,))
    t1.start()
    t1.join()

if __name__ == "__main__":
    main()