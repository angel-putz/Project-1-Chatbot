#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SCENARIOS 10

unsigned int randomSeed = 97342; // On utilise une graine fixe

typedef struct
{
    char nom[30];
    int sante;
    int moral;
    int nourriture; // Quantit� de nourriture poss�d�e par le personnage
    int soin;       // Ressource plus rare et utilisable par la soigneuse uniquement
    int faim;       // Cette variable faiblit lorsque le joueur ne mange pas, chaque tour fais baisser cette variable
    int tours;      // Variable pour le nombre total de tours pass�s
    int cuistot;
    int soigneur;

} Personnage;       // Sutrure utile pour chaque personnage cr��

typedef struct {
    int id;
    char description[200];
    int effetNourriture;
    int effetSante;
    int effetMoral;
    int suivants[MAX_SCENARIOS]; // Liste des �v�nements suivants
    int nbSuivants; // Nombre d'�v�nements suivants
} Event;


void intro();
unsigned int simple_random();                   // Une seed g�n�rer semi-al�atoirement � chaque nouvelle run id�alement, va permettre de g�n�rer les probabilit�s de la RUN,
void menuInteraction(Personnage *pp, Event events[], int nbEvents, int *currentEvent);          // Va contenir les actions disponibles par l'utilisateur: manger/bouger/soigner/chercher)
void actionBouger(Personnage *pp, Event events[], int nbEvents, int *currentEvent);             // Bouger va permettre de generer un event qui poss�de diff�rente finalit�s en fonctions de probalit�s, cete action coute un tour(+1) et du moral (-5)
void actionChercher(Personnage *pp);           // Cette action va engager une variable chance, qui va permettre de trouver ou pas des ressources
void soigner(Personnage *pp);            // Cette fontion va permettre de ragagner de la sant�
void manger(Personnage *pp);             // Cette fonction permet de combler la faim et de remonter le moral
void evenementPnj(Personnage *pp);             // Fonction pour g�rer les �v�nements PNJ avec choix
void afficherRessources(const Personnage *pp); // Cette fonction permet � l'utilisateur de voir ses ressources disponibles
int contientMotClef();
int chargerEvenements(const char *filename, Event *events, int maxEvents);




int main() {
    // Initialisation du personnage
    Personnage pp = {"Aventurier", 100, 100, 5, 1, 10, 0, 0, 0};

    // Charger les �v�nements
    Event events[100]; // Tableau d'�v�nements
    int nbEvents = chargerEvenements("story.txt", events, 100); // Charger depuis le fichier
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



void intro()
{
    printf("   _____ __________________  _________     _____  .____    _____.___.__________  ____________________ \n");
    printf("  /  _  \\______    \\_____  \\ \\_   ___ \\   /  _  \\ |    |   \\__  |   |\\______   \\/   _____/\\_   _____/ \n");
    printf(" /  /_\\  \\|     ___//   |   \\/    \\  \\/  /  /_\\  \\|    |    /   |   | |     ___/\\_____  \\  |    __)_  \n");
    printf("/    |    \\    |   /    |    \\     \\____/    |    \\    |___ \\____   | |    |    /        \\ |        \\ \n");
    printf("\\____|__  /____|   \\_______  /\\______  /\\____|__  /_______ \\/ ______| |____|   /_______  //_______  / \n");
    printf("        \\/                 \\/        \\/         \\/        \\/\\/                         \\/         \\/  \n");

    printf("\n\n*** Bienvenue dans APOCALYPSE : Survive or Die ***\n\n");

    printf("L'ann�e est 2043. Une �pid�mie incontr�lable a transform� la majeure partie de la population\n");
    printf("en zombies assoiff�s de sang. Le monde tel que nous le connaissions a disparu, et seuls les\n");
    printf("plus courageux ou les plus chanceux ont surv�cu.\n\n");

    printf("Vous faites partie d'un groupe de survivants, barricad�s dans un abri temporaire, mais\n");
    printf("les ressources commencent � manquer. Chaque jour est une lutte pour la survie et chaque\n");
    printf("d�cision peut �tre la derni�re.\n\n");

    printf("Explorez les zones dangereuses, trouvez des ressources vitales, et prot�gez vos compagnons.\n");
    printf("Les zombies r�dent, et des d�cisions cruciales vous attendent.\n\n");

    printf(">>> Pr�parez-vous � faire face � l'apocalypse. Saurez-vous survivre ? <<<\n\n");
}

unsigned int simple_random()
{
    // Param�tres de LCG : ces valeurs fonctionnent bien pour g�n�rer des nombres pseudo-al�atoires
    randomSeed = (randomSeed * 1103515245 + 12345) & 0x7fffffff;
    return randomSeed;
}


// Fonction qui v�rifie si un des synonymes est pr�sent dans la phrase de l'utilisateur
void menuInteraction(Personnage *pp, Event events[], int nbEvents, int *currentEvent) {
    // Tableaux de synonymes pour diff�rentes actions
    const char *tabManger[] = {"manger", "repas", "nourriture"};
    const char *tabBouger[] = {"marcher", "bouger", "d�placer"};
    const char *tabChercher[] = {"chercher", "trouver", "explorer"};
    const char *tabSoigner[] = {"soigner", "gu�rir", "traiter"};
    const char *tabAfficher[] = {"afficher", "montrer", "voir"};

    char choix[100]; // Stocke la r�ponse de l'utilisateur

    // Demander � l'utilisateur d'entrer une action
    printf("\nQue souhaitez-vous faire ? D�crivez votre action : ");
    fgets(choix, sizeof(choix), stdin);

    // Retirer le saut de ligne de la cha�ne si pr�sent
    choix[strcspn(choix, "\n")] = '\0';

    // V�rifier et ex�cuter l'action en fonction des mots-cl�s
    if (contientMotClef(choix, tabManger, sizeof(tabManger) / sizeof(tabManger[0]))) {
        manger(pp);
    }
    else if (contientMotClef(choix, tabBouger, sizeof(tabBouger) / sizeof(tabBouger[0]))) {
        actionBouger(pp, events, nbEvents, currentEvent);
        pp->tours += 2; // Compte pour 2 tours
    }
    else if (contientMotClef(choix, tabChercher, sizeof(tabChercher) / sizeof(tabChercher[0]))) {
        actionChercher(pp);
        pp->tours++; // Compte pour 1 tour
    }
    else if (contientMotClef(choix, tabSoigner, sizeof(tabSoigner) / sizeof(tabSoigner[0]))) {
        soigner(pp);
    }
    else if (contientMotClef(choix, tabAfficher, sizeof(tabAfficher) / sizeof(tabAfficher[0]))) {
        afficherRessources(pp);
    }
    else {
        printf("Choix invalide. Veuillez r�essayer.\n");
    }

    // R�duire la faim du personnage apr�s chaque action
    pp->faim -= 1;
}


void manger (Personnage *p){
     if (p->cuistot == 1) {
            if (p->nourriture >= 1 && p->faim > 0) {
                p->faim -= 5;
                if (p->faim < 0) p->faim = 0;
                p->nourriture--; // Prend 1 nourriture
                printf("%s a mang� avec le cuistot. Faim : %d, Nourriture restante : %d\n", p->nom, p->faim, p->nourriture);
                p->tours++;  // Compte pour 1 tour
            } else {
                printf("Pas assez de nourriture ou %s n'a pas faim.\n", p->nom);
            }
        } else if (p->cuistot == 0) {
            if (p->nourriture >= 5) {
                p->faim -= 5;
                if (p->faim < 0) p->faim = 0;
                p->nourriture -= 5; // Prend 5 nourritures
                printf("Pas de cuistot. %s a mang� mais a consomm� plus de nourriture. Faim : %d, Nourriture restante : %d\n", p->nom, p->faim, p->nourriture);
                p->tours++;  // Compte pour 1 tour
            }
            else {
                printf("Pas assez de nourriture pour %s.\n", p->nom);
            }
        }

}

void soigner(Personnage *p){
     if (p->soigneur == 1) {
            if (p->soin >= 1 && p->sante < 100) {
                p->sante += 5; // Soigne 5 PV
                if (p->sante > 100) p->sante = 100;
                p->soin--; // Prend 1 soin
                printf("%s a �t� soign� avec le soigneur. Sant� : %d, Soins restants : %d\n", p->nom, p->sante, p->soin);
                p->tours++;  // Compte pour 1 tour

            } else {
                printf("Pas assez de soins ou %s est en pleine sant�.\n", p->nom);
            }
        } else if (p->soigneur == 0) {
            if (p->soin >= 2) {
                p->sante += 3; // Soigne 3 PV
                if (p->sante > 100) p->sante = 100;
                p->soin -= 2; // Prend 2 soins
                printf("Pas de soigneur. %s a �t� soign� partiellement. Sant� : %d, Soins restants : %d\n", p->nom, p->sante, p->soin);
                p->tours++;  // Compte pour 1 tour
            } else {
                printf("Pas assez de soins pour %s.\n", p->nom);
            }
        }

}

void evenementPnj(Personnage *pp)
{
    int evenement = simple_random() % 10;
}


// Fonction pour charger les �v�nements depuis story.txt
int chargerEvenements(const char *filename, Event events[], int maxEvents) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier d'�v�nements");
        return 0; // Retourner 0 si le fichier ne peut pas �tre ouvert
    }

    int i = 0; // Indice pour remplir le tableau d'�v�nements
    while (i < maxEvents && fscanf(file, " %[^\n]s", events[i].description) == 1) {
        events[i].id = i; // Donner un identifiant unique � chaque �v�nement
        i++;
    }

    fclose(file); // Fermer le fichier apr�s lecture
    return i; // Retourner le nombre d'�v�nements charg�s
}

void actionBouger(Personnage *pp, Event events[], int nbEvents, int *currentEvent) {
    if (*currentEvent >= nbEvents) {
        printf("Plus d'�v�nements disponibles � explorer.\n");
        return; // Retourner si tous les �v�nements ont �t� parcourus
    }

    // Afficher l'�v�nement actuel
    printf("�v�nement : %s\n", events[*currentEvent].description);

    // Augmenter l'indice pour passer � l'�v�nement suivant
    (*currentEvent)++;
}

// Fonction qui v�rifie si un des synonymes est pr�sent dans la phrase de l'utilisateur
int contientMotClef(const char *phrase, const char *synonymes[], int taille)
{
    for (int i = 0; i < taille; i++)
    {
        if (strstr(phrase, synonymes[i]) != NULL)
        {
            return 1; // Le mot-cl� a �t� trouv� dans la phrase
        }
    }
    return 0; // Aucun mot-cl� trouv�
}
{

}

void actionChercher(Personnage *pp)
{
    int chance = simple_random() % 100;
    if (chance < 50)
    { // Si la chance g�n�rer par la seed est inf�rieur � 50, alors le joueur gagne des ressources
        pp->nourriture += 2;
        printf("%s a trouv� de la nourriture !\n", pp->nom);
    }
    else
    {
        printf("%s n'a rien trouv� cette fois.\n", pp->nom);
    }
    if (chance < 10)
    {
        pp->soin += 1;
        printf("%s a trouv� du soin ! C'est rare !\n", pp->nom);
    }
}

void afficherRessources(const Personnage *pp)
{
    printf("\n--- Ressources de %s ---\n", pp->nom);
    printf("Sant�      : %d\n", pp->sante);
    printf("Moral      : %d\n", pp->moral);
    printf("Nourriture : %d\n", pp->nourriture);
    printf("Soins      : %d\n", pp->soin);
    printf("Faim       : %d\n", pp->faim);
    printf("Tours      : %d\n", pp->tours);
    printf("--------------------------\n");
}
