#include <semaphore.h>

// ① Déclarer
sem_t semaphore;

// ② Initialiser
sem_init(&semaphore, 0, 3);
//            ↑      ↑  ↑
//         adresse   │  valeur initiale (3 places)
//                   0 = partagé entre threads
//                   1 = partagé entre processus

// ③ Entrer (décrémenter)
sem_wait(&semaphore);
//  compteur--
//  si compteur < 0 → BLOQUE et attend

// ④ Sortir (incrémenter)
sem_post(&semaphore);
//  compteur++
//  réveille un thread en attente

// ⑤ Détruire
sem_destroy(&semaphore);