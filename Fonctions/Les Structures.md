Les Structures

typedef struct {
    char nom[30];
    int age;
    int sante;
    int degat;
} Personnage;

typedef struct {
    char nom[30];
    int sante;
    int bouffe;
    bool maladie;
} MATE;

typedef struct {
    int degat;
    int sante
} ZOMBIE;

// Variables globales pour les ressources
int nourriture = 10;
int eau = 10;