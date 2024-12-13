#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SCENARIOS 10

unsigned int randomSeed = 97342; // On utilise une graine fixe

typedef struct {
    char nom[30];
    int sante;
    int moral;
    int nourriture; // Quantité de nourriture possédée par le personnage
    int soin;       // Ressource plus rare et utilisable par la soigneuse uniquement
    int faim;       // Cette variable faiblit lorsque le joueur ne mange pas
    int tours;      // Variable pour le nombre total de tours passés
    int cuistot;
    int soigneur;
} Personnage;

typedef struct {
    int id;
    char description[200];
    int effetNourriture;
    int effetSante;
    int effetMoral;
    int effetCuistot;
    int effetSoigneur;
    int suivants[MAX_SCENARIOS]; // Liste des événements suivants
    int nbSuivants;             // Nombre d'événements suivants
} Event;

void intro();
unsigned int simple_random();
void menuInteraction(Personnage *pp, Event events[], int nbEvents, int *currentEvent);
void actionBouger(Personnage *pp, Event events[], int nbEvents, int *currentEvent);
void actionChercher(Personnage *pp);
void soigner(Personnage *pp);
void manger(Personnage *pp);
void afficherRessources(const Personnage *pp);
int contientMotClef(const char *phrase, const char *synonymes[], int taille);
int chargerEvenementsEtAppliquer(const char *filename, Event *events, int maxEvents, Personnage *pp);

int main() {
    // Initialisation du personnage
    Personnage pp = {"Aventurier", 100, 100, 5, 1, 10, 0, 0, 0};
    intro();

    // Charger les événements
    Event events[100]; // Tableau d'événements
    int nbEvents = chargerEvenementsEtAppliquer("story.txt", events, 100, &pp); // Charger depuis le fichier
    int currentEvent = 0; // Premier événement

    // Boucle principale du jeu
    while (pp.sante > 0) {
        printf("\n--- Début du tour %d ---\n", pp.tours + 1);

        // Interagir avec le joueur
        menuInteraction(&pp, events, nbEvents, &currentEvent);

        // Vérifier si le personnage a trop faim
        if (pp.faim <= 0) {
            pp.sante -= 5; // Réduire la santé si la faim atteint 0
            printf("Attention ! %s a trop faim et perd de la santé.\n", pp.nom);
        }
    }

    // Fin du jeu
    printf("Votre personnage n'a plus de santé. Fin du jeu.\n");
    return 0;
}

void intro() {
    printf("\n*** Bienvenue dans APOCALYPSE : Survive or Die ***\n");
    printf("Saurez-vous survivre dans ce monde post-apocalyptique ?\n\n");
}

unsigned int simple_random() {
    randomSeed = (randomSeed * 1103515245 + 12345) & 0x7fffffff;
    return randomSeed;
}

int chargerEvenementsEtAppliquer(const char *filename, Event *events, int maxEvents, Personnage *pp) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier d'événements");
        return 0;
    }

    int i = 0;
    while (i < maxEvents) {
        char ligne[300];
        if (!fgets(ligne, sizeof(ligne), file)) break;

        char *description = strtok(ligne, "|");
        char *effetStr = strtok(NULL, "\n");

        if (description) {
            strncpy(events[i].description, description, sizeof(events[i].description) - 1);
        }

        if (effetStr) {
            sscanf(effetStr, "Sante:%d,Nourriture:%d,Moral:%d,Cuistot:%d,Soigneur:%d",
                   &events[i].effetSante, &events[i].effetNourriture, &events[i].effetMoral, &events[i].effetCuistot, &events[i].effetSoigneur);
        }

        i++;
    }
    printf("Chargement de l'événement %d : %s\n", i, events[i].description);
    printf("Effets : Santé = %d, Nourriture = %d, Moral = %d\n",
       events[i].effetSante, events[i].effetNourriture, events[i].effetMoral);


    fclose(file);
    return i;
}

void menuInteraction(Personnage *pp, Event events[], int nbEvents, int *currentEvent) {
    const char *tabManger[] = {"manger", "repas", "nourriture"};
    const char *tabBouger[] = {"marcher", "bouger", "déplacer"};
    const char *tabChercher[] = {"chercher", "trouver", "explorer"};
    const char *tabSoigner[] = {"soigner", "guérir", "traiter"};
    const char *tabAfficher[] = {"afficher", "montrer", "voir"};

    char choix[100];
    printf("\nQue souhaitez-vous faire ? Décrivez votre action : ");
    fgets(choix, sizeof(choix), stdin);
    choix[strcspn(choix, "\n")] = '\0';

    if (contientMotClef(choix, tabManger, sizeof(tabManger) / sizeof(tabManger[0]))) {
        manger(pp);
    } else if (contientMotClef(choix, tabBouger, sizeof(tabBouger) / sizeof(tabBouger[0]))) {
        actionBouger(pp, events, nbEvents, currentEvent);
        pp->tours += 2;
    } else if (contientMotClef(choix, tabChercher, sizeof(tabChercher) / sizeof(tabChercher[0]))) {
        actionChercher(pp);
        pp->tours++;
    } else if (contientMotClef(choix, tabSoigner, sizeof(tabSoigner) / sizeof(tabSoigner[0]))) {
        soigner(pp);
    } else if (contientMotClef(choix, tabAfficher, sizeof(tabAfficher) / sizeof(tabAfficher[0]))) {
        afficherRessources(pp);
    } else {
        printf("Choix invalide. Veuillez réessayer.\n");
    }

    pp->faim -= 1;
}

void actionBouger(Personnage *pp, Event events[], int nbEvents, int *currentEvent) {
    if (*currentEvent >= nbEvents) {
        printf("Plus d'événements disponibles à explorer.\n");
        return;
    }

    Event *event = &events[*currentEvent];

    // Débogage avant l'application de l'événement
    printf("Avant l'événement : Santé = %d, Nourriture = %d, Moral = %d\n",
           pp->sante, pp->nourriture, pp->moral);

    // Affichage de l'événement
    printf("Événement : %s Effet : Nourriture %+d, Moral %+d, Santé %+d\n",
           event->description, event->effetNourriture, event->effetMoral, event->effetSante);

    // Mise à jour des ressources
    pp->sante += event->effetSante;
    pp->nourriture += event->effetNourriture;
    pp->moral += event->effetMoral;
    pp->cuistot += event->effetCuistot;
    pp->soigneur += event->effetSoigneur;

    // Vérification des limites des ressources
    if (pp->sante > 100) pp->sante = 100;
    if (pp->sante < 0) pp->sante = 0;
    if (pp->moral > 100) pp->moral = 100;
    if (pp->moral < 0) pp->moral = 0;

    // Débogage après l'application de l'événement
    printf("Après l'événement : Santé = %d, Nourriture = %d, Moral = %d\n",
           pp->sante, pp->nourriture, pp->moral);

    (*currentEvent)++; // Passer à l'événement suivant
}


void actionChercher(Personnage *pp) {
    int chance = simple_random() % 100;
    if (chance < 50) {
        pp->nourriture += 2;
        printf("%s a trouvé de la nourriture !\n", pp->nom);
    } else {
        printf("%s n'a rien trouvé cette fois.\n", pp->nom);
    }

    if (chance < 10) {
        pp->soin += 1;
        printf("%s a trouvé un soin rare !\n", pp->nom);
    }
}

void soigner(Personnage *p) {
    if (p->soigneur == 1 && p->soin > 0) {
        p->sante = (p->sante + 5 > 100) ? 100 : p->sante + 5;
        p->soin--;
        printf("%s a été soigné. Santé : %d\n", p->nom, p->sante);
    } else if (p->soin >= 2) {
        p->sante = (p->sante + 3 > 100) ? 100 : p->sante + 3;
        p->soin -= 2;
        printf("%s a été partiellement soigné. Santé : %d\n", p->nom, p->sante);
    } else {
        printf("Pas assez de soins disponibles.\n");
    }
}

void manger(Personnage *p) {
    if (p->nourriture > 0) {
        p->faim = (p->faim - 5 < 0) ? 0 : p->faim + 5;
        p->nourriture--;
        printf("%s a mangé. Faim : %d, Nourriture restante : %d\n", p->nom, p->faim, p->nourriture);
    } else {
        printf("Pas assez de nourriture disponible.\n");
    }
}

void afficherRessources(const Personnage *pp) {
    printf("\n--- Ressources de %s ---\n", pp->nom);
    printf("Santé      : %d\n", pp->sante);
    printf("Moral      : %d\n", pp->moral);
    printf("Nourriture : %d\n", pp->nourriture);
    printf("Soins      : %d\n", pp->soin);
    printf("Faim       : %d\n", pp->faim);
    printf("Tours passés : %d\n", pp->tours);
}

int contientMotClef(const char *phrase, const char *synonymes[], int taille) {
    for (int i = 0; i < taille; i++) {
        if (strstr(phrase, synonymes[i])) {
            return 1;
        }
    }
    return 0;
}
