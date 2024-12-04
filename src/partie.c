/*
@author = clementfavarel
*/

#define TAILLE_NOM_PARTIE 31
#define TAILLE_PSEUDO 21

typedef struct
{
    int id;
    char nom[TAILLE_NOM_PARTIE];
    int utilisateurId;
} PARTIE;

// Fonctions du fichier
int compterNombreParties(FILE *bddParties);
int verifierExistencePartie(FILE *bddParties, const char *nomPartieAVerifier);
void creerPartie(FILE *bddParties, FILE *bddUtilisateurs);
void commencerAventure(PARTIE *partie);
// PARTIE rechercherPartieParNom(FILE *bddParties, const char *nomCherche);
// void reprendrePartie(FILE *bddParties);

// Dépendances
int verifierExistencePseudo(FILE *bddUtilisateurs, const char *pseudoAVerifier);
UTILISATEUR rechercherUtilisateurParNom(FILE *bddUtilisateurs, const char *pseudoCherche);

int compterNombreParties(FILE *bddParties)
{
    PARTIE partie;
    int nombreParties = 0;

    fseek(bddParties, 0, SEEK_SET);
    while (fread(&partie, sizeof(PARTIE), 1, bddParties))
    {
        nombreParties = nombreParties + 1;
    }

    return nombreParties;
}

int verifierExistencePartie(FILE *bddParties, const char *nomPartieAVerifier)
{
    PARTIE partie;
    fseek(bddParties, 0, SEEK_SET);
    while (fread(&partie, sizeof(PARTIE), 1, bddParties))
    {
        if (strcmp(partie.nom, nomPartieAVerifier) == 0)
        {
            return 1;
        }
    }

    return 0;
}

void creerPartie(FILE *bddParties, FILE *bddUtilisateurs)
{
    PARTIE partie;
    int nombreParties = compterNombreParties(bddParties);
    int partie_existe = 0;

    char pseudo[TAILLE_PSEUDO];
    printf("Entrez votre pseudo : ");
    scanf("%s", pseudo);

    // Vérifier l'existence du pseudo
    int utilisateurExiste = verifierExistencePseudo(bddUtilisateurs, pseudo);

    if (!utilisateurExiste)
    {
        char choix;
        printf("L'utilisateur n'existe pas. Voulez-vous créer un nouvel utilisateur ? (O/N) : ");
        scanf(" %c", &choix);

        if (choix == 'O' || choix == 'o')
        {
            // Créer un nouvel utilisateur
            UTILISATEUR nouvelUtilisateur;
            strcpy(nouvelUtilisateur.pseudo, pseudo);

            // Obtenir le prochain ID disponible
            int nombreUtilisateurs = compterNombreUtilisateurs(bddUtilisateurs);
            nouvelUtilisateur.id = nombreUtilisateurs + 1;

            // Écrire le nouvel utilisateur dans le fichier
            fseek(bddUtilisateurs, 0, SEEK_END);
            if (fwrite(&nouvelUtilisateur, sizeof(UTILISATEUR), 1, bddUtilisateurs) != 1)
            {
                perror("Erreur lors de la création de l'utilisateur");
                return;
            }

            // Utiliser le nouvel utilisateur pour la partie
            partie.utilisateurId = nouvelUtilisateur.id;
        }
        else
        {
            printf("Création de partie annulée.\n");
            return;
        }
    }
    else
    {
        // Si l'utilisateur existe déjà, le rechercher
        UTILISATEUR utilisateur = rechercherUtilisateurParNom(bddUtilisateurs, pseudo);
        partie.utilisateurId = utilisateur.id;
    }

    printf("Entrez le nom de la partie : ");
    scanf("%s", partie.nom);

    partie_existe = verifierExistencePartie(bddParties, partie.nom);

    if (partie_existe)
    {
        printf("Ce nom de partie est déjà utilisé par une autre partie, veuillez en choisir un autre ou reprenez cette partie.\n");
    }
    else
    {
        partie.id = nombreParties + 1;

        fseek(bddParties, 0, SEEK_END);
        if (fwrite(&partie, sizeof(PARTIE), 1, bddParties) != 1)
        {
            perror("Erreur lors de l'écriture dans le fichier");
        }
        else
        {
            commencerAventure(&partie);
        }
    }
}
