// Lib for booleans
#include <stdbool.h>

// Variables globales à utiliser avec ces structures
#define FAIM 10
#define SOIF 10
#define PV 30

// Personnages Non Jouables (PNJ)
typedef struct {
    char nom[30];
    int age;
    int sante;
    int degat;
} PNJ;

// Personnages de l'histoire (coéquipiers)
typedef struct {
    char nom[30];
    int sante;
    int bouffe;
    bool maladie;
} COEQUIPIER;

// Zombies
typedef struct {
    int degat;
    int sante
} ZOMBIE;