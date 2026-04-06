import threading
import random

def ma_fonction(result):
    n = 5
    t = [random.randint(0, 98) for _ in range(n)]
    for value in t:
        print(value, end=" ")
    print()
    result.append(t)

def main():
    result = []
    t1 = threading.Thread(target=ma_fonction, args=(result,))
    t1.start()
    t1.join()
    tableau_original = result[0] if result else None

if __name__ == "__main__":
    main()