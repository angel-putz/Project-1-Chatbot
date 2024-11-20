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
        printf("L'utilisateur n'existe pas.\n");
        return;
    }

    UTILISATEUR utilisateur = rechercherUtilisateurParNom(bddUtilisateurs, pseudo);
    partie.utilisateurId = utilisateur.id;

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
            printf("Les informations ont été enregistrées avec succès.\n");
        }
    }

    return;
}