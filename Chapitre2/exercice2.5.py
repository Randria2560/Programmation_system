import os
import sys
import time
import random

def main():

    # ① Durées aléatoires entre 1 et 10 secondes
    duree1 = random.randint(1, 10)
    duree2 = random.randint(1, 10)

    print("=== Démarrage ===")
    print(f"Fils 1 va dormir {duree1} secondes")
    print(f"Fils 2 va dormir {duree2} secondes")
    print(f"Le plus long est le fils "
          f"{'1' if duree1 > duree2 else '2'} "
          f"({max(duree1, duree2)} secondes)\n")

    # ② Mémoriser le temps de départ
    debut = time.time()

    # ③ Créer fils 1
    pid1 = os.fork()
    if pid1 == -1:
        print("Erreur fork fils 1")
        sys.exit(1)

    if pid1 == 0:
        # ── FILS 1 ──────
        print(f"[Fils 1] démarre (PID = {os.getpid()})")
        time.sleep(duree1)
        print(f"[Fils 1] terminé après {duree1} secondes")
        sys.exit(0)

    # ④ Créer fils 2
    pid2 = os.fork()
    if pid2 == -1:
        print("Erreur fork fils 2")
        sys.exit(1)

    if pid2 == 0:
        # ── FILS 2 ─
        print(f"[Fils 2] démarre (PID = {os.getpid()})")
        time.sleep(duree2)
        print(f"[Fils 2] terminé après {duree2} secondes")
        sys.exit(0)

    # ── PÈRE ──

    # ⑤ Attendre les deux fils
    for i in range(2):
        pid_termine, _ = os.wait()

        if pid_termine == pid1:
            print("[Père] fils 1 terminé")
        elif pid_termine == pid2:
            print("[Père] fils 2 terminé")

    # ⑥ Calculer la durée totale
    fin           = time.time()
    duree_totale  = fin - debut

    print("\n=== Résultat ===")
    print(f"Durée totale   : {duree_totale:.1f} secondes")
    print(f"Plus long fils : {max(duree1, duree2)} secondes")

if __name__ == "__main__":
    main()