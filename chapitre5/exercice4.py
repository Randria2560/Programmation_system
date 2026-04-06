import threading
import random
import time
import sys

def generation(tab):
    print(f"Génération d'un tableau de {tab['n']} éléments...")
    tab['t'] = [random.randint(0, 99) for _ in range(tab['n'])]
    time.sleep(1)

def t_inclusion(p, result):
    est_inclus = True
    for i in range(p['t1']['n']):
        trouve = False
        for j in range(p['t2']['n']):
            if p['t1']['t'][i] == p['t2']['t'][j]:
                trouve = True
                break
        if not trouve:
            est_inclus = False
            break
    result['value'] = est_inclus

def t_annulation(stop_event):
    while not stop_event.is_set():
        c = sys.stdin.read(1)
        if c.lower() == 'a':
            print("\n[ANNULATION] Arrêt du programme par l'utilisateur.")
            sys.exit(0)

def main():
    random.seed()

    while True:
        try:
            entry = input("Taille T1 et T2 (T1 doit être <= T2) : ")
            n1, n2 = map(int, entry.split())
            if n1 <= n2:
                break
        except ValueError:
            pass

    data1 = {'n': n1, 't': []}
    data2 = {'n': n2, 't': []}

    thread1 = threading.Thread(target=generation, args=(data1,))
    thread2 = threading.Thread(target=generation, args=(data2,))
    thread1.start()
    thread2.start()
    thread1.join()
    thread2.join()

    total = {
        't1': data1,
        't2': data2
    }

    result = {'value': False}
    stop_event = threading.Event()

    recherche = threading.Thread(target=t_inclusion, args=(total, result))
    annulation = threading.Thread(target=t_annulation, args=(stop_event,), daemon=True)

    recherche.start()
    annulation.start()

    recherche.join()
    stop_event.set()

    print(f"Résultat du calcul : {'Inclus' if result['value'] else 'Non inclus'}")

if __name__ == "__main__":
    main()