#include <stdio.h>

int main() {

    long nb_plateaux = 2;
    long faces_par_plateau = 2;
    long nb_faces = nb_plateaux * faces_par_plateau; // 4 faces
    long cylindres_par_face = 1000;
    long secteurs_par_cylindre = 60;
    long octets_par_secteur = 1024;

    //  a) Capacité totale ---
    long capacite_totale = nb_faces * cylindres_par_face * secteurs_par_cylindre * octets_par_secteur;
    printf("a) Capacite totale du disque : %ld octets\n", capacite_totale);

    //  b) Position de l'octet spécifique ---
    // Octet 300, secteur 45, cylindre 350, face 2, p//====6.1============================
//====6.2============================
//====6.3.a============================
//====6.3.b============================lateau 1
    long cylindre_dest = 350;
    long face_dest = 1;    // Face 2 (index 1)
    long secteur_dest = 45;
    long octet_dest = 300;

    long pos_absolue = (cylindre_dest * nb_faces * secteurs_par_cylindre * octets_par_secteur) +
                       (face_dest * secteurs_par_cylindre * octets_par_secteur) +
                       (secteur_dest * octets_par_secteur) +
                       octet_dest;
    
    printf("b) Position de l'octet : %ld\n", pos_absolue);

    //c) Localisation de l'octet 78 000 000
    long cible = 78000000;
    
    long reste = cible;
    long res_cylindre = reste / (nb_faces * secteurs_par_cylindre * octets_par_secteur);
    reste %= (nb_faces * secteurs_par_cylindre * octets_par_secteur);

    long res_face = reste / (secteurs_par_cylindre * octets_par_secteur);
    reste %= (secteurs_par_cylindre * octets_par_secteur);

    long res_secteur = reste / octets_par_secteur;
    long res_pos_secteur = reste % octets_par_secteur;

    printf("c) L'octet 78 000 000 se trouve :\n");
    printf("   - Cylindre : %ld\n", res_cylindre);
    printf("   - Face : %ld\n", res_face);
    printf("   - Secteur : %ld\n", res_secteur);
    printf("   - Position dans le secteur : %ld\n", res_pos_secteur);

    return 0;
}