def main():
    # Données de l'exercice 6.1 [1]
    nb_plateaux = 2
    faces_par_plateau = 2
    nb_faces = nb_plateaux * faces_par_plateau  # 4 faces
    cylindres_par_face = 1000
    secteurs_par_cylindre = 60
    octets_par_secteur = 1024

    # --- a) Capacité totale ---
    capacite_totale = nb_faces * cylindres_par_face * secteurs_par_cylindre * octets_par_secteur
    print(f"a) Capacite totale du disque : {capacite_totale} octets")

    # --- b) Position de l'octet spécifique ---
    # Octet 300, secteur 45, cylindre 350, face 2, plateau 1
    cylindre_dest = 350
    face_dest = 1    # Face 2 (index 1)
    secteur_dest = 45
    octet_dest = 300

    # Calcul de la position absolue (offset)
    pos_absolue = (cylindre_dest * nb_faces * secteurs_par_cylindre * octets_par_secteur) + \
                   (face_dest * secteurs_par_cylindre * octets_par_secteur) + \
                   (secteur_dest * octets_par_secteur) + \
                   octet_dest
    
    print(f"b) Position de l'octet : {pos_absolue}")

    # --- c) Localisation de l'octet 78 000 000 ---
    cible = 78000000
    
    reste = cible
    # Utilisation de // pour la division entière
    res_cylindre = reste // (nb_faces * secteurs_par_cylindre * octets_par_secteur)
    reste %= (nb_faces * secteurs_par_cylindre * octets_par_secteur)

    res_face = reste // (secteurs_par_cylindre * octets_par_secteur)
    reste %= (secteurs_par_cylindre * octets_par_secteur)

    res_secteur = reste // octets_par_secteur
    res_pos_secteur = reste % octets_par_secteur

    print("c) L'octet 78 000 000 se trouve :")
    print(f"   - Cylindre : {res_cylindre}")
    print(f"   - Face : {res_face}")
    print(f"   - Secteur : {res_secteur}")
    print(f"   - Position dans le secteur : {res_pos_secteur}")

if __name__ == "__main__":
    main()