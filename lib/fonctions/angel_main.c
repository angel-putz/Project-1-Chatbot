#define MAX_SCENARIOS 10

unsigned int random_seed = 97342; // On utilise une graine fixe
int eau = 10;

const char *tab_manger[] = {"manger", "repas", "nourriture"};
const char *tab_bouger[] = {"marcher", "bouger", "déplacer"};
const char *tab_chercher[] = {"chercher", "trouver", "explorer"};
const char *tab_soigner[] = {"soigner", "guérir", "traiter"};
const char *tab_afficher[] = {"afficher", "montrer", "voir"};

typedef struct
{
    char nom[30];
    int sante;
    int moral;
    int nourriture; // Quantite de nourriture possedee par le personnage
    int soin;       // Ressource plus rare et utilisable par la soigneuse uniquement
    int faim;       // Cette variable faiblit lorsque le joueur ne mange pas, chaque tour fais baisser cette variable
    int tours;      // Variable pour le nombre total de tours passes
} Personnage;       // Sutrure utile pour chaque personnage cree

typedef struct
{
    int type;              // Type d'evenement, identifiant unique
    char description[200]; // Description de l'evenement
    int effet_nourriture;  // Effet sur la nourriture
    int effet_sante;       // Effet sur la sante
    int effet_moral;       // Effet sur le moral
    int effet_tours;       // Effet sur les tours
    int used;              // Pour savoir si le scenario est dejà apparu
} Scenario;

int main(int argc, char **argv)
{
    intro();
    menu();

    while (pp.sante > 0)
    {
        printf("\n--- Debut du tour %d (tour max : 100)---\n", pp.tours + 1); // Affichage du tour actuel
        printf("\n---Que Souhaitez-Vous Faire ?");
        menu_interaction(&pp); // Afficher les interractions

        if (pp.faim <= 0)
        { // Gestion de la faim et consequences sur la sante
            pp.sante -= 5;
            printf("Attention ! %s a trop faim et perd de la sante.\n", pp.nom);
        }
    }

    printf("Votre personnage n'a plus de sante. Fin du jeu.\n");
    return 0;
}

unsigned int simple_random()
{
    // Paramètres de LCG : ces valeurs fonctionnent bien pour generer des nombres pseudo-aleatoires
    random_seed = (random_seed * 1103515245 + 12345) & 0x7fffffff;
    return random_seed;
}

// Fonction qui vérifie si un des synonymes est présent dans la phrase de l'utilisateur
int contient_mot_clef(const char *phrase, const char *synonymes[], int taille)
{
    for (int i = 0; i < taille; i++)
    {
        if (strstr(phrase, synonymes[i]) != NULL)
        {
            return 1; // Le mot-clé a été trouvé dans la phrase
        }
    }
    return 0; // Aucun mot-clé trouvé
}

void menu_interaction(Personnage *pp)
{
    char choix[100]; // Tableau pour stocker la réponse de l'utilisateur

    printf("\nQue souhaitez-vous faire ? Décrivez votre action : ");
    fgets(choix, sizeof(choix), stdin);

    // Retirer le saut de ligne de la fin de la chaîne si présent
    choix[strcspn(choix, "\n")] = '\0';

    // Vérifier chaque action en utilisant le tableau de synonymes correspondant et afficher la partie d'histoire liée
    if (contient_mot_clef(choix, tab_manger, sizeof(tab_manger) / sizeof(tab_manger[0])))
    {
        action_manger(pp);
        pp->tours++; // Compte pour 1 tour
        afficher_histoire("manger");
    }
    else if (contient_mot_clef(choix, tab_bouger, sizeof(tab_bouger) / sizeof(tab_bouger[0])))
    {
        action_bouger(pp);
        pp->tours += 2; // Compte pour 2 tours
        afficher_histoire("bouger");
    }
    else if (contient_mot_clef(choix, tab_chercher, sizeof(tab_chercher) / sizeof(tab_chercher[0])))
    {
        action_chercher(pp);
        pp->tours++; // Compte pour 1 tour
        afficher_histoire("chercher");
    }
    else if (contient_mot_clef(choix, tab_soigner, sizeof(tab_soigner) / sizeof(tab_soigner[0])))
    {
        action_soigner(pp);
        pp->tours++; // Compte pour 1 tour
        afficher_histoire("soigner");
    }
    else if (contient_mot_clef(choix, tab_afficher, sizeof(tab_afficher) / sizeof(tab_afficher[0])))
    {
        afficher_ressources(pp);
        afficher_histoire("afficher");
    }
    else
    {
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
        // on doit faire un truc
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

// Nouvelle fonction pour gérer un combat contre un zombie
void combat_zombie(Personnage *pp)
{
    int pv_zombie = 15;     // Points de vie du zombie, relativement faibles pour ne pas rendre le jeu trop difficile
    int attaque_zombie = 3; // Points de dégâts que le zombie inflige par attaque

    printf("Un zombie vous attaque ! Le combat commence...\n");

    while (pv_zombie > 0 && pp->sante > 0)
    {
        // Le personnage attaque le zombie
        int degats_infliges = 5; // Par exemple, des dégâts fixes pour simplifier
        pv_zombie -= degats_infliges;
        printf("%s attaque le zombie et inflige %d points de dégâts !\n", pp->nom, degats_infliges);

        if (pv_zombie <= 0)
        {
            printf("Le zombie est vaincu !\n");
            break; // Sort du combat si le zombie est mort
        }

        // Le zombie contre-attaque
        printf("Le zombie vous attaque et inflige %d points de dégâts !\n", attaque_zombie);
        degats(pp, attaque_zombie);

        if (pp->sante <= 0)
        {
            printf("Vous êtes gravement blessé et devez fuir !\n");
            break; // Sort du combat si le personnage n'a plus de santé
        }
    }
}