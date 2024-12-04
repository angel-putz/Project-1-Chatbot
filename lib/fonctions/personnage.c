#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SCENARIOS 10

unsigned int randomSeed = 97342; // On utilise une graine fixe

EVENT EFFET NOURRITURE EST VIDE CEST POURQUOI LADDITION NE FONCTIONNE PAS

typedef struct {
    char nom[30];
    int sante;
    int moral;
    int nourriture; // Quantit� de nourriture poss�d�e par le personnage
    int soin;       // Ressource plus rare et utilisable par la soigneuse uniquement
    int faim;       // Cette variable faiblit lorsque le joueur ne mange pas
    int tours;      // Variable pour le nombre total de tours pass�s
    int cuistot;
    int soigneur;
} Personnage;

typedef struct {
    int id;
    char description[200];
    int effetNourriture;
    int effetSante;
    int effetMoral;
    int suivants[MAX_SCENARIOS]; // Liste des �v�nements suivants
    int nbSuivants;             // Nombre d'�v�nements suivants
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

    // Charger les �v�nements
    Event events[100]; // Tableau d'�v�nements
    int nbEvents = chargerEvenementsEtAppliquer("story.txt", events, 100, &pp); // Charger depuis le fichier
    int currentEvent = 0; // Premier �v�nement

    // Boucle principale du jeu
    while (pp.sante > 0) {
        printf("\n--- D�but du tour %d ---\n", pp.tours + 1);

        // Interagir avec le joueur
        menuInteraction(&pp, events, nbEvents, &currentEvent);

        // V�rifier si le personnage a trop faim
        if (pp.faim <= 0) {
            pp.sante -= 5; // R�duire la sant� si la faim atteint 0
            printf("Attention ! %s a trop faim et perd de la sant�.\n", pp.nom);
        }
    }

    // Fin du jeu
    printf("Votre personnage n'a plus de sant�. Fin du jeu.\n");
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
        perror("Erreur lors de l'ouverture du fichier d'�v�nements");
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
            sscanf(effetStr, "Sante:%d,Nourriture:%d,Moral:%d",
                   &events[i].effetSante, &events[i].effetNourriture, &events[i].effetMoral);
        }

        i++;
    }
    printf("Chargement de l'�v�nement %d : %s\n", i, events[i].description);
    printf("Effets : Sant� = %d, Nourriture = %d, Moral = %d\n",
       events[i].effetSante, events[i].effetNourriture, events[i].effetMoral);


    fclose(file);
    return i;
}

void menuInteraction(Personnage *pp, Event events[], int nbEvents, int *currentEvent) {
    const char *tabManger[] = {"manger", "repas", "nourriture"};
    const char *tabBouger[] = {"marcher", "bouger", "d�placer"};
    const char *tabChercher[] = {"chercher", "trouver", "explorer"};
    const char *tabSoigner[] = {"soigner", "gu�rir", "traiter"};
    const char *tabAfficher[] = {"afficher", "montrer", "voir"};

    char choix[100];
    printf("\nQue souhaitez-vous faire ? D�crivez votre action : ");
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
        printf("Choix invalide. Veuillez r�essayer.\n");
    }

    pp->faim -= 1;
}

void actionBouger(Personnage *pp, Event events[], int nbEvents, int *currentEvent) {
    if (*currentEvent >= nbEvents) {
        printf("Plus d'�v�nements disponibles � explorer.\n");
        return;
    }

    Event *event = &events[*currentEvent];

    // D�bogage avant l'application de l'�v�nement
    printf("Avant l'�v�nement : Sant� = %d, Nourriture = %d, Moral = %d\n",
           pp->sante, pp->nourriture, pp->moral);

    // Affichage de l'�v�nement
    printf("�v�nement : %s Effet : Nourriture %+d, Moral %+d, Sant� %+d\n",
           event->description, event->effetNourriture*2, event->effetMoral*2, event->effetSante*2);

    // Mise � jour des ressources
    pp->sante += event->effetSante;
    pp->nourriture += event->effetNourriture;
    pp->moral += event->effetMoral;

    // V�rification des limites des ressources
    if (pp->sante > 100) pp->sante = 100;
    if (pp->sante < 0) pp->sante = 0;
    if (pp->moral > 100) pp->moral = 100;
    if (pp->moral < 0) pp->moral = 0;

    // D�bogage apr�s l'application de l'�v�nement
    printf("Apr�s l'�v�nement : Sant� = %d, Nourriture = %d, Moral = %d\n",
           pp->sante, pp->nourriture, pp->moral);

    (*currentEvent)++; // Passer � l'�v�nement suivant
}


void actionChercher(Personnage *pp) {
    int chance = simple_random() % 100;
    if (chance < 50) {
        pp->nourriture += 2;
        printf("%s a trouv� de la nourriture !\n", pp->nom);
    } else {
        printf("%s n'a rien trouv� cette fois.\n", pp->nom);
    }

    if (chance < 10) {
        pp->soin += 1;
        printf("%s a trouv� un soin rare !\n", pp->nom);
    }
}

void soigner(Personnage *p) {
    if (p->soigneur == 1 && p->soin > 0) {
        p->sante = (p->sante + 5 > 100) ? 100 : p->sante + 5;
        p->soin--;
        printf("%s a �t� soign�. Sant� : %d\n", p->nom, p->sante);
    } else if (p->soin >= 2) {
        p->sante = (p->sante + 3 > 100) ? 100 : p->sante + 3;
        p->soin -= 2;
        printf("%s a �t� partiellement soign�. Sant� : %d\n", p->nom, p->sante);
    } else {
        printf("Pas assez de soins disponibles.\n");
    }
}

void manger(Personnage *p) {
    if (p->nourriture > 0) {
        p->faim = (p->faim - 5 < 0) ? 0 : p->faim - 5;
        p->nourriture--;
        printf("%s a mang�. Faim : %d, Nourriture restante : %d\n", p->nom, p->faim, p->nourriture);
    } else {
        printf("Pas assez de nourriture disponible.\n");
    }
}

void afficherRessources(const Personnage *pp) {
    printf("\n--- Ressources de %s ---\n", pp->nom);
    printf("Sant�      : %d\n", pp->sante);
    printf("Moral      : %d\n", pp->moral);
    printf("Nourriture : %d\n", pp->nourriture);
    printf("Soins      : %d\n", pp->soin);
    printf("Faim       : %d\n", pp->faim);
    printf("Tours pass�s : %d\n", pp->tours);
}

int contientMotClef(const char *phrase, const char *synonymes[], int taille) {
    for (int i = 0; i < taille; i++) {
        if (strstr(phrase, synonymes[i])) {
            return 1;
        }
    }
    return 0;
}
