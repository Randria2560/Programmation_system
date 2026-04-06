import os
import signal
import sys
import time

# Nombre de fils à créer [1, 2]
NB_FILS = 5
# Tableau pour stocker les PID des fils [3, 4]
pids =  * NB_FILS
# Variable pour aider le gestionnaire à identifier le rôle (Père ou Fils)
est_parent = True

def handle_sigterm(sig, frame):
    """Gestionnaire de signal pour le message de fin [5, 6]"""
    self_pid = os.getpid() # [3, 7]
    if not est_parent:
        print(f"[Fils {self_pid}] meurs")
        sys.exit(0)
    else:
        print(f"[Père {self_pid}] tué->Fin du programme.")
        sys.exit(0)

def fils_process(index):
    """Fonction exécutée par chaque fils [3, 8]"""
    global est_parent
    est_parent = False
    
    # Chaque fils installe son gestionnaire pour SIGTERM [6, 9]
    signal.signal(signal.SIGTERM, handle_sigterm)
    
    print(f"Fils {index} (PID: {os.getpid()}) démarré.")
    while True:
        # Boucle infinie demandée [2]
        time.sleep(1) 

def main():
    global pids
    # Installation du gestionnaire pour le père [6]
    signal.signal(signal.SIGTERM, handle_sigterm)

    # Création des 5 fils via fork() [1, 8]
    for i in range(NB_FILS):
        pid = os.fork()
        if pid == 0:
            # Code du processus fils [3]
            fils_process(i)
            sys.exit(0)
        else:
            # Code du processus père : on stocke le PID du fils [3, 4]
            pids[i] = pid

    # Boucle de menu pour le père [2]
    while True:
        try:
            print(f"\n--- MENU PERE (Mon PID: {os.getpid()}) ---")
            print("1. Endormir un fils (SIGSTOP)")
            print("2. Réveiller un fils (SIGCONT)")
            print("3. Terminer un fils (SIGTERM)")
            print("4. Quitter (Tue le père et les fils)")
            
            choix_input = input("Choix : ")
            if not choix_input: continue
            choix = int(choix_input)

            if choix == 4:
                # Le père termine tous ses fils avant de se tuer lui-même [4, 10]
                for i in range(NB_FILS):
                    try:
                        os.kill(pids[i], signal.SIGTERM)
                    except ProcessLookupError:
                        pass
                os.kill(os.getpid(), signal.SIGTERM)

            num_fils_input = input(f"Numéro du fils (0 à {NB_FILS - 1}) : ")
            if not num_fils_input: continue
            num_fils = int(num_fils_input)

            if num_fils < 0 or num_fils >= NB_FILS:
                print("Fils invalide.")
                continue

            target_pid = pids[num_fils]

            # Envoi des signaux selon le choix [4, 10]
            if choix == 1:
                os.kill(target_pid, signal.SIGSTOP) # Suspendre [11]
                print(f"Fils {num_fils} suspendu.")
            elif choix == 2:
                os.kill(target_pid, signal.SIGCONT) # Reprendre [11]
                print(f"Fils {num_fils} réveillé.")
            elif choix == 3:
                os.kill(target_pid, signal.SIGTERM) # Terminer [9]
                print(f"Demande de terminaison envoyée au fils {num_fils}.")
            else:
                print("Option inconnue.")
                
        except (ValueError, EOFError):
            print("\nEntrée invalide ou fin de programme.")
            break

if __name__ == "__main__":
    main()