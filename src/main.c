#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define TAILLE_MAX_MDP 21
#define MOT_DE_PASSE_ADMIN "Admin123!"

void intro();
void menu();
void demander_mdp();
void menu_admin();

int main(int argc, char **argv) {
    intro();

    menu();

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
    
    printf("L'année est 2043. Une épidémie incontrôlable a transformé la majeure partie de la population\n");
    printf("en zombies assoiffés de sang. Le monde tel que nous le connaissions a disparu, et seuls les\n");
    printf("plus courageux ou les plus chanceux ont survécu.\n\n");

    printf("Vous faites partie d'un groupe de survivants, barricadés près d'un bunker, mais\n");
    printf("les ressources commencent à manquer. Chaque jour est une lutte pour la survie et chaque\n");
    printf("décision peut être la dernière.\n\n");

    printf("Explorez les zones dangereuses, trouvez des ressources vitales, et protégez vos compagnons.\n");
    printf("Les zombies rôdent, et des décisions cruciales vous attendent.\n\n");

    printf(">>> Préparez-vous à faire face à l'apocalypse. Saurez-vous survivre ? <<<\n\n");
    return;
}

void menu()
{
    char choix = ' ';

    while (choix != 'Q')
    {
        printf("\n*** Menu principal ***\n");
        printf("Q) Quitter\n");
        printf("C) Créer une partie\n");
        printf("R) Reprendre une partie\n");
        printf("A) Mode Admin\n");
        printf("\nVotre choix : ");
        scanf(" %c", &choix); // Lecture du caractère choisi par l'utilisateur
        getchar(); // Vider le tampon d'entrée pour enlever le caractère '\n'

        switch (choix)
        {
            case 'Q':
                printf("\nFermeture du programme.\n");
                break;
            case 'c':
            case 'C':
                printf("\nCréer une partie.\n");
                break;
            case 'r':
            case 'R':
                printf("\nReprendre une partie.\n");
                break;
            case 'a':
            case 'A':
                printf("\nMode Admin.\n");
                demander_mdp();
                break;
            default:
                printf("\nChoix invalide, assurez-vous de saisir un caractère valide\n");
        }
    }
    return;
}

void demander_mdp()
{
    char mdp[TAILLE_MAX_MDP];
    int tentatives = 3;

    while (tentatives > 0) {
        printf("\nVeuillez entrer votre mot de passe : ");

        // Désactiver l'affichage des caractères dans le terminal
        struct termios oldt, newt;
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
            menu_admin();
            return;  // Sortie de la fonction si le mot de passe est correct
        } else {
            printf("\nMot de passe incorrect. Il vous reste %d tentative(s).\n", tentatives - 1);
        }

        tentatives--;  // Décrémenter le nombre de tentatives restantes
    }

    // Si toutes les tentatives sont épuisées
    printf("Erreur : trop de tentatives échouées. Accès refusé.\n");
}

void menu_admin()
{
    char choix = ' ';

    while (choix != 'Q')
    {
        printf("\n*** Menu Admin ***\n");
        printf("Q) Retour au menu principal\n");
        printf("U) Gérer Utilisateurs\n");
        printf("\nVotre choix : ");
        scanf(" %c", &choix); // Lecture du caractère choisi par l'utilisateur
        getchar(); // Vider le tampon d'entrée pour enlever le caractère '\n'

        switch (choix)
        {
            case 'Q':
                printf("\nRetour au menu principal.\n");
                break;
            default:
                printf("\nChoix invalide, assurez-vous de saisir un caractère valide\n");
        }
    }
    return;
}