import os
import sys
import shutil
import errno

def main():

    while True:
        # ① Saisir le nom du répertoire destination
        destination = input("\nEntrez le nom du répertoire "
                           "(ou 'q' pour quitter) : ").strip()

        # ② Vérifier si l'utilisateur veut quitter
        if destination == "q":
            break

        # ③ Vérifier que le nom n'est pas vide
        if not destination:
            print("Nom vide, veuillez recommencer.")
            continue

        # ④ Créer le répertoire destination
        try:
            os.mkdir(destination, 0o755)
            print(f"Répertoire '{destination}' créé.")
        except FileExistsError:
            print(f"Répertoire '{destination}' existe déjà.")
        except OSError as e:
            print(f"Erreur mkdir : {e}")
            continue

        # ⑤ Créer un fils pour effectuer la copie
        pid = os.fork()

        if pid == -1:
            print("Erreur fork")
            continue

        if pid == 0:
            # ── FILS : copier le répertoire courant 
            try:
                # Copier tout le répertoire courant vers destination
                source = os.getcwd()
                for item in os.listdir(source):
                    # Ne pas copier le dossier destination dans lui-même
                    if item == destination:
                        continue
                    src  = os.path.join(source, item)
                    dest = os.path.join(destination, item)
                    if os.path.isdir(src):
                        shutil.copytree(src, dest)
                    else:
                        shutil.copy2(src, dest)
                sys.exit(0)   # succès

            except Exception as e:
                print(f"Erreur copie : {e}")
                sys.exit(1)   # échec

        else:
            # ── PÈRE : attendre la fin de la copie ──
            pid_termine, status = os.waitpid(pid, 0)

            # Vérifier si le fils s'est terminé normalement
            if os.WIFEXITED(status) and os.WEXITSTATUS(status) == 0:
                print(f"Copie vers '{destination}' terminée.")
            else:
                print(f"Erreur lors de la copie vers '{destination}'.")

    print("\nFin du programme.")

if __name__ == "__main__":
    main()