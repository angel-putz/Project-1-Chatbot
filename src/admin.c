/*
@author = clementfavarel
*/

#define TAILLE_MAX_MDP 21
#define MOT_DE_PASSE_ADMIN "Admin123!"

void demanderMdpAdmin();
void afficherMenuAdmin();
void afficherMenuUtilisateur();

void demanderMdpAdmin()
{
    struct termios oldt, newt;
    char mdp[TAILLE_MAX_MDP];
    int tentatives = 3;

    while (tentatives > 0) {
        printf("\nVeuillez entrer votre mot de passe : ");

        // Désactiver l'affichage des caractères dans le terminal
        tcgetattr(STDIN_FILENO, &oldt);  // Obtenir les paramètres actuels du terminal
        newt = oldt;
        newt.c_lflag &= ~ECHO;           // Désactiver l'affichage des caractères
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        // Lecture du mot de passe
        if (fgets(mdp, TAILLE_MAX_MDP, stdin) != NULL) {
            mdp[strcspn(mdp, "\n")] = '\0'; // Retirer le retour à la ligne
        }

        // Rétablir les paramètres du terminal
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

        // Vérification du mot de passe
        if (strlen(mdp) == 0) {
            printf("\nErreur : le mot de passe ne peut pas être vide.\n\n");
            continue;
        } else if (strcmp(mdp, MOT_DE_PASSE_ADMIN) == 0) {
            // mdp correct \n\n
            afficherMenuAdmin();
            return;  // Sortie de la fonction si le mot de passe est correct
        } else {
            printf("\nMot de passe incorrect. Il vous reste %d tentative(s).\n", tentatives - 1);
        }

        tentatives--;  // Décrémenter le nombre de tentatives restantes
    }

    // Si toutes les tentatives sont épuisées
    printf("Erreur : trop de tentatives échouées. Accès refusé.\n");
}

void afficherMenuAdmin()
{
    char choix = ' ';

    while (choix != 'R')
    {
        printf("\n\n*** Menu Admin ***\n");
        printf("R) Retour au menu principal\n");
        printf("U) Gérer les utilisateurs\n");
        printf("\nVotre choix : ");
        scanf(" %c", &choix); // Lecture du caractère choisi par l'utilisateur
        getchar(); // Vider le tampon d'entrée pour enlever le caractère '\n'

        switch (choix)
        {
            case 'R':
                printf("\nRetour au menu principal.\n");
                break;
            case 'u':
            case 'U':
                printf("\nGérer les utilisateurs\n");
                afficherMenuUtilisateur();
                break;
            default:
                printf("\nChoix invalide, assurez-vous de saisir un caractère valide\n");
                break;
        }
    }
    return;
}