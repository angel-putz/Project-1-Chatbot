/*
@author = clementfavarel
*/

#define NOM_DOSSIER_BDD "bdd"
#define NOM_FICHIER_UTILISATEURS "utilisateurs.bin"
#define NOM_FICHIER_PARTIES "parties.bin"

void afficherTexteIntro();
void afficherMenuPrincipal();
FILE *ouvrirFichierDansDossier(char *nomDossier, char *nomFichier);
void creerPartie(FILE *bddParties);
void fermerFichier(FILE *fichierOuvert);
void demanderMdpAdmin();

void afficherTexteIntro()
{
    // Clear le terminal avant d'afficher le texte d'intro
    printf("\e[1;1H\e[2J");

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

void afficherMenuPrincipal()
{
    FILE *bddUtilisateurs = NULL;
    bddUtilisateurs = ouvrirFichierDansDossier(NOM_DOSSIER_BDD, NOM_FICHIER_UTILISATEURS);

    FILE *bddParties = NULL;
    bddParties = ouvrirFichierDansDossier(NOM_DOSSIER_BDD, NOM_FICHIER_PARTIES);

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
        getchar();            // Vider le tampon d'entrée pour enlever le caractère '\n'

        switch (choix)
        {
        case 'Q':
            printf("\nFermeture du programme.\n");
            break;
        case 'c':
        case 'C':
            printf("\nCréer une partie.\n");
            creerPartie(bddParties);
            break;
        case 'r':
        case 'R':
            printf("\nReprendre une partie.\n");
            break;
        case 'a':
        case 'A':
            printf("\nMode Admin.\n");
            demanderMdpAdmin();
            break;
        default:
            printf("\nChoix invalide, assurez-vous de saisir un caractère valide\n");
            break;
        }
    }
    return;

    fermerFichier(bddUtilisateurs);
    fermerFichier(bddParties);
}