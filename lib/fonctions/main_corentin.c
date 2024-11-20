#include <stdio.h>

#define MAX_SCENARIOS 10

unsigned int random_seed = 97342; // On utilise une graine fixe
int eau = 10;

typedef struct
{
    char nom[30];
    int sante;
    int moral;
    int nourriture; // Quantité de nourriture possédée par le personnage
    int soin;       // Ressource plus rare et utilisable par la soigneuse uniquement
    int faim;       // Cette variable faiblit lorsque le joueur ne mange pas, chaque tour fais baisser cette variable
    int tours;      // Variable pour le nombre total de tours passés
} Personnage;       // Sutrure utile pour chaque personnage créé

typedef struct
{
    int type;              // Type d'événement, identifiant unique
    char description[200]; // Description de l'événement
    int effet_nourriture;  // Effet sur la nourriture
    int effet_sante;       // Effet sur la santé
    int effet_moral;       // Effet sur le moral
    int effet_tours;       // Effet sur les tours
    int used;              // Pour savoir si le scénario est déjà apparu
} Scenario;

void intro();
unsigned int simple_random();                   // Une seed générer semi-aléatoirement à chaque nouvelle run idéalement, va permettre de générer les probabilités de la RUN,
void menu_interaction(Personnage *pp);          // Va contenir les actions disponibles par l'utilisateur: manger/bouger/soigner/chercher)
void action_bouger(Personnage *pp);             // Bouger va permettre de generer un event qui possède différente finalités en fonctions de probalités, cete action coute un tour(+1) et du moral (-5)
void action_chercher(Personnage *pp);           // Cette action va engager une variable chance, qui va permettre de trouver ou pas des ressources
void action_soigner(Personnage *pp);            // Cette fontion va permettre de ragagner de la santé
void action_manger(Personnage *pp);             // Cette fonction permet de combler la faim et de remonter le moral
void evenement_pnj(Personnage *pp);             // Fonction pour gérer les événements PNJ avec choix
void afficher_ressources(const Personnage *pp); // Cette fonction permet à l'utilisateur de voir ses ressources disponibles

int main(int argc, char **argv)
{
    intro();
    Personnage pp = {"Aventurier", 100, 100, 5, 1, 10, 0}; // Initialisation du personnage avec 0 tours
    intro();

    while (pp.sante > 0)
    {
        printf("\n--- Début du tour %d (tour max : 100)---\n", pp.tours + 1); // Affichage du tour actuel
        printf("\n---Que Souhaitez-Vous Faire ?");
        menu_interaction(&pp); // Afficher les interractions

        if (pp.faim <= 0)
        { // Gestion de la faim et conséquences sur la santé
            pp.sante -= 5;
            printf("Attention ! %s a trop faim et perd de la santé.\n", pp.nom);
        }
    }

    printf("Votre personnage n'a plus de santé. Fin du jeu.\n");
    return 0;
}

unsigned int simple_random()
{
    // Paramètres de LCG : ces valeurs fonctionnent bien pour générer des nombres pseudo-aléatoires
    random_seed = (random_seed * 1103515245 + 12345) & 0x7fffffff;
    return random_seed;
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

    printf("L'année est 2043. Une épidémie incontrôlable a transformé la majeure partie de la population\n");
    printf("en zombies assoiffés de sang. Le monde tel que nous le connaissions a disparu, et seuls les\n");
    printf("plus courageux ou les plus chanceux ont survécu.\n\n");

    printf("Vous faites partie d'un groupe de survivants, barricadés dans un abri temporaire, mais\n");
    printf("les ressources commencent à manquer. Chaque jour est une lutte pour la survie et chaque\n");
    printf("décision peut être la dernière.\n\n");

    printf("Explorez les zones dangereuses, trouvez des ressources vitales, et protégez vos compagnons.\n");
    printf("Les zombies rôdent, et des décisions cruciales vous attendent.\n\n");

    printf(">>> Préparez-vous à faire face à l'apocalypse. Saurez-vous survivre ? <<<\n\n");
}

void menu_interaction(Personnage *pp)
{
    int choix;

    printf("\n--- Menu d'Interaction ---\n");
    printf("1. Manger\n");
    printf("2. Bouger\n");
    printf("3. Chercher des ressources\n");
    printf("4. Se soigner\n");
    printf("5. Afficher vos ressources\n");
    printf("Votre choix : ");
    scanf("%d", &choix);

    switch (choix)
    {
    case 1:
        action_manger(pp);
        pp->tours++; // Compte pour 1 tour
        break;
    case 2:
        action_bouger(pp);
        pp->tours += 2; // Compte pour 2 tours
        break;
    case 3:
        action_chercher(pp);
        pp->tours++; // Compte pour 1 tour
        break;
    case 4:
        action_soigner(pp);
        pp->tours++; // Compte pour 1 tour
    case 5:
        afficher_ressources(pp);
        break;
    default:
        printf("Choix invalide. Veuillez réessayer.\n");
    }

    pp->faim -= 1; // La faim diminue à chaque action
}

void action_manger(Personnage *pp)
{
    if (pp->nourriture > 0)
    {
        pp->nourriture--;
        pp->faim = 10;  // La faim est restaurée
        pp->moral += 5; // Gain de moral en mangeant
        printf("%s a mangé et se sent mieux.\n", pp->nom);
    }
    else
    {
        printf("Pas assez de nourriture pour manger.\n");
    }
}

void evenement_pnj(Personnage *pp)
{
    int evenement = simple_random() % 10;
}

void action_bouger(Personnage *pp)
{
    printf("%s bouge vers une nouvelle zone...\n", pp->nom);
    pp->moral -= 5; // Bouger coûte du moral

    // Utilisation de simple_random pour générer un "événement"
    int evenement = simple_random() % 100; // Obtenir un nombre entre 0 et 99

    if (evenement < 40)
    {
        printf("Un zombie apparaît soudainement ! Vous perdez de la santé en vous échappant.\n");
        pp->sante -= 10;
    }
    else if (evenement < 80)
    {
        printf("Vous rencontrez un PNJ amical.\n");
        evenement_pnj(pp); // Appelle la fonction pour un événement PNJ avec choix
    }
    else
    {
        printf("Vous trouvez un campement abandonné avec des provisions !\n");
        pp->nourriture += 2;
        eau += 1;
    }
}

void action_soigner(Personnage *pp)
{
    if (pp->soin > 0)
    {
        pp->soin--;
        pp->sante += 15;
        printf("%s utilise du soin et regagne de la santé.\n", pp->nom);
    }
    else
    {
        printf("Pas assez de soin pour se soigner.\n");
    }
}

void action_chercher(Personnage *pp)
{
    int chance = simple_random() % 100;
    if (chance < 50)
    { // Si la chance générer par la seed est inférieur à 50, alors le joueur gagne de l'eau et des ressources
        pp->nourriture += 2;
        eau += 1;
        printf("%s a trouvé de la nourriture et de l'eau !\n", pp->nom);
    }
    else
    {
        printf("%s n'a rien trouvé cette fois.\n", pp->nom);
    }
    if (chance < 10)
    {
        pp->soin += 1;
        printf("%s a trouvé du soin ! C'est rare !\n", pp->nom);
    }
}

void afficher_ressources(const Personnage *pp)
{
    printf("\n--- Ressources de %s ---\n", pp->nom);
    printf("Santé      : %d\n", pp->sante);
    printf("Moral      : %d\n", pp->moral);
    printf("Nourriture : %d\n", pp->nourriture);
    printf("Soins      : %d\n", pp->soin);
    printf("Faim       : %d\n", pp->faim);
    printf("Tours      : %d\n", pp->tours);
    printf("--------------------------\n");
}