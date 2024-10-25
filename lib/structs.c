// Lib for booleans
#include <stdbool.h>

// Variables globales à utiliser avec ces structures
#define OUI 0
#define NON 1
#define TAILLE_NOM 51
#define TAILLE_MESSAGE 1001

// Personnages Non Jouables (PNJ)
typedef struct {
    char nom[TAILLE_NOM];
    char message[TAILLE_MESSAGE];
    bool mort;
} PNJ;

// Personnages de l'histoire (coéquipiers)
typedef struct {
    char nom[TAILLE_NOM];
    int pv;
    int faim;
    bool sante;
} PERSONNAGE;

// Zombies
typedef struct {
    int pv;
    int degat;
    bool mort;
} ZOMBIE;