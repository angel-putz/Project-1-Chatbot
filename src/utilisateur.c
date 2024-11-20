/*
@author = clementfavarel
*/

#define TAILLE_PSEUDO 21
#define OUI 0
#define NON 1
#define NOM_DOSSIER_BDD "bdd"
#define NOM_FICHIER_UTILISATEURS "utilisateurs.bin"

typedef struct
{
    int id;
    char pseudo[TAILLE_PSEUDO];
} UTILISATEUR;

void afficherMenuUtilisateur();
void afficherUtilisateur(UTILISATEUR *utilisateur);
void listerUtilisateurs(FILE *bddUtilisateurs);
int compterNombreUtilisateurs(FILE *bddUtilisateurs);
int verifierExistencePseudo(FILE *bddUtilisateurs, const char *pseudoAVerifier);
void creerUtilisateur(FILE *bddUtilisateurs);
UTILISATEUR rechercherUtilisateurParNom(FILE *bddUtilisateurs, const char *pseudoCherche);
void consulterUtilisateur(FILE *bddUtilisateurs);
void modifierUtilisateur(FILE *bddUtilisateurs);
void supprimerUtilisateur(FILE *bddUtilisateurs);

void afficherMenuUtilisateur()
{
    FILE *bddUtilisateurs = NULL;
    bddUtilisateurs = ouvrirFichierDansDossier(NOM_DOSSIER_BDD, NOM_FICHIER_UTILISATEURS);

    UTILISATEUR utilisateur;

    char choix = ' ';

    while (choix != 'R')
    {
        printf("\n*** Gestion des Utilisateurs ***\n");
        printf("R) Retour au menu admin\n");
        printf("L) Lister les utilisateurs\n");
        printf("C) Consulter les informations d'un utilisateur\n");
        printf("A) Ajouter un utilisateur\n");
        printf("M) Modifier un utilisateur\n");
        printf("S) Supprimer un utilisateur\n");
        printf("\nVotre choix : ");
        scanf(" %c", &choix); // Lecture du caractère choisi par l'utilisateur
        getchar();            // Vider le tampon d'entrée pour enlever le caractère '\n'

        switch (choix)
        {
        case 'R':
            printf("\nRetour au menu admin.\n");
            break;
        case 'l':
        case 'L':
            printf("\nLister les utilisateurs\n");
            listerUtilisateurs(bddUtilisateurs);
            break;
        case 'c':
        case 'C':
            printf("\nConsulter les informations d'un utilisateur\n");
            consulterUtilisateur(bddUtilisateurs);
            break;
        case 'a':
        case 'A':
            printf("\nAjouter un utilisateur\n");
            creerUtilisateur(bddUtilisateurs);
            break;
        case 'm':
        case 'M':
            printf("\nModifier un utilisateur\n");
            modifierUtilisateur(bddUtilisateurs);
            break;
        case 's':
        case 'S':
            printf("\nSupprimer un utilisateur\n");
            supprimerUtilisateur(bddUtilisateurs);
            break;
        default:
            printf("\nChoix invalide, assurez-vous de saisir un caractère valide\n");
            break;
        }
    }

    fermerFichier(bddUtilisateurs);
    return;
}

void afficherUtilisateur(UTILISATEUR *utilisateur)
{
    printf("\nInformations de l'utilisateur :\n");
    printf("  - ID : %d\n", utilisateur->id);
    printf("  - Pseudo : %s\n", utilisateur->pseudo);
}

void listerUtilisateurs(FILE *bddUtilisateurs)
{
    printf("\nListe des utilisateurs:\n");

    UTILISATEUR utilisateur;
    int utilisateurTrouve = 0; // Indicateur pour savoir si au moins un utilisateur a été trouvé

    fseek(bddUtilisateurs, 0, SEEK_SET);
    while (fread(&utilisateur, sizeof(UTILISATEUR), 1, bddUtilisateurs))
    {
        afficherUtilisateur(&utilisateur);
        utilisateurTrouve = 1; // On a trouvé au moins un utilisateur
    }

    if (!utilisateurTrouve)
    {
        printf("Il n'y a aucun utilisateur enregistré pour l'instant.\n");
    }
}

int compterNombreUtilisateurs(FILE *bddUtilisateurs)
{
    UTILISATEUR utilisateur;
    int nombreUtilisateurs = 0;

    fseek(bddUtilisateurs, 0, SEEK_SET);
    while (fread(&utilisateur, sizeof(UTILISATEUR), 1, bddUtilisateurs))
    {
        nombreUtilisateurs = nombreUtilisateurs + 1;
    }

    return nombreUtilisateurs;
}

int verifierExistencePseudo(FILE *bddUtilisateurs, const char *pseudoAVerifier)
{
    UTILISATEUR utilisateur;
    fseek(bddUtilisateurs, 0, SEEK_SET);
    while (fread(&utilisateur, sizeof(UTILISATEUR), 1, bddUtilisateurs))
    {
        if (strcmp(utilisateur.pseudo, pseudoAVerifier) == 0)
        {
            return 1; // Le pseudo existe déjà
        }
    }
    return 0; // Le pseudo n'existe pas
}

void creerUtilisateur(FILE *bddUtilisateurs)
{
    UTILISATEUR utilisateur;
    int nombreUtilisateurs = compterNombreUtilisateurs(bddUtilisateurs);
    int pseudoExiste = 0;

    // Saisie des informations de la personne
    printf("Entrez votre pseudo : ");
    scanf("%s", utilisateur.pseudo);

    // Vérifier si le pseudo existe déjà
    pseudoExiste = verifierExistencePseudo(bddUtilisateurs, utilisateur.pseudo);

    if (pseudoExiste)
    {
        printf("Ce pseudo est déjà utilisé par un autre utilisateur, veuillez en choisir un autre.\n");
    }
    else
    {
        // Initialisation des valeurs par défaut
        utilisateur.id = nombreUtilisateurs + 1;

        // Écriture de la personne dans le fichier
        fseek(bddUtilisateurs, 0, SEEK_END);
        if (fwrite(&utilisateur, sizeof(UTILISATEUR), 1, bddUtilisateurs) != 1)
        {
            perror("Erreur lors de l'écriture dans le fichier");
        }
        else
        {
            printf("Les informations ont été enregistrées avec succès.\n");
        }
    }
}

UTILISATEUR rechercherUtilisateurParNom(FILE *bddUtilisateurs, const char *pseudoCherche)
{
    UTILISATEUR utilisateur;

    fseek(bddUtilisateurs, 0, SEEK_SET);
    while (fread(&utilisateur, sizeof(UTILISATEUR), 1, bddUtilisateurs))
    {
        if (strcmp(utilisateur.pseudo, pseudoCherche) == 0)
        {
            return utilisateur; // Retourner la personne trouvée
        }
    }

    printf("Personne avec le nom '%s' non trouvée dans le fichier.\n", pseudoCherche);
    UTILISATEUR utilisateurNonTrouvee;
    memset(&utilisateurNonTrouvee, 0, sizeof(UTILISATEUR));
    return utilisateurNonTrouvee;
}

void consulterUtilisateur(FILE *bddUtilisateurs)
{
    char pseudoCherche[TAILLE_PSEUDO];
    UTILISATEUR utilisateur;

    printf("Entrez le pseudo de l'utilisateur à consulter : ");
    scanf("%s", pseudoCherche);

    utilisateur = rechercherUtilisateurParNom(bddUtilisateurs, pseudoCherche);

    if (utilisateur.id != 0)
    {
        afficherUtilisateur(&utilisateur);
    }
    else
    {
        printf("Utilisateur non trouvé.\n");
    }
}

void modifierUtilisateur(FILE *bddUtilisateurs)
{
    char pseudoCherche[TAILLE_PSEUDO];
    UTILISATEUR utilisateurAModifier;

    printf("Entrez le pseudo de l'utilisateur à modifier : ");
    scanf("%s", pseudoCherche);

    utilisateurAModifier = rechercherUtilisateurParNom(bddUtilisateurs, pseudoCherche);

    if (utilisateurAModifier.id != 0)
    { // Utilisateur trouvé
        // Affichage des informations actuelles de l'utilisateur
        afficherUtilisateur(&utilisateurAModifier);

        // Demander les modifications
        printf("Entrez le nouveau pseudo (laissez vide pour ne pas modifier) : ");
        scanf("%s", pseudoCherche);
        if (strlen(pseudoCherche) > 0)
        {
            strcpy(utilisateurAModifier.pseudo, pseudoCherche);
        }

        // Réécrire l'utilisateur modifié dans le fichier
        fseek(bddUtilisateurs, -sizeof(UTILISATEUR), SEEK_CUR); // Repositionner le curseur
        fwrite(&utilisateurAModifier, sizeof(UTILISATEUR), 1, bddUtilisateurs);
        printf("Utilisateur modifié avec succès.\n");
    }
    else
    {
        printf("Utilisateur non trouvé.\n");
    }
}

void supprimerUtilisateur(FILE *bddUtilisateurs)
{
    char pseudoCherche[TAILLE_PSEUDO];
    UTILISATEUR utilisateurASupprimer;

    printf("Entrez le pseudo de l'utilisateur à supprimer : ");
    scanf("%s", pseudoCherche);

    utilisateurASupprimer = rechercherUtilisateurParNom(bddUtilisateurs, pseudoCherche);

    if (utilisateurASupprimer.id != 0)
    { // Utilisateur trouvé
        // Supprimer l'utilisateur en écrasant avec le prochain utilisateur
        UTILISATEUR utilisateurSuivant;
        while (fread(&utilisateurSuivant, sizeof(UTILISATEUR), 1, bddUtilisateurs))
        {
            fseek(bddUtilisateurs, -2 * sizeof(UTILISATEUR), SEEK_CUR); // Repositionner le curseur
            fwrite(&utilisateurSuivant, sizeof(UTILISATEUR), 1, bddUtilisateurs);
        }
        if (ftruncate(fileno(bddUtilisateurs), ftell(bddUtilisateurs) - sizeof(UTILISATEUR)) != 0)
        {
            perror("Erreur lors du tronquage du fichier");
        }
        else
        {
            printf("Utilisateur supprimé avec succès.\n");
        }
    }
    else
    {
        printf("Utilisateur non trouvé.\n");
    }
}