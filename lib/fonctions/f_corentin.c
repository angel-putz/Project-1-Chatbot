

void menu_interaction(Personnage *pp)
{
    int choix;

    printf("\n--- Menu d'Interaction ---\n");
    printf("1. Demander de Manger\n");
    printf("2. Bouger\n");
    printf("3. Chercher des ressources\n");
    printf("4. Demande de soin\n");
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

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

void evenement_pnj(Personnage *pp)
{
    int evenement = simple_random() % 10;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

void action_bouger(Personnage *pp)
{
    printf("%s bouge vers une nouvelle zone...\n", pp->nom);
    pp->moral -= 5; // Bouger coûte du moral

    // Utilisation de simple_random pour générer un "événement"
    int evenement = simple_random() % 100; // Obtenir un nombre entre 0 et 99

    if (evenement < 40)
    {
        combat_zombie(pp);
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
void degats(Personnage *pp, int points)
{
    pp->sante -= points;
    if (pp->sante < 0)
        pp->sante = 0;
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

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------

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