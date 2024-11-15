/*
@author = clementfavarel
*/

#define TAILLE_NOM_PARTIE 31

typedef struct
{
    int id;
    char nom[TAILLE_NOM_PARTIE];
    int utilisateurId;
}PARTIE;

int compterNombreParties(FILE *bdd_parties);
int verifierExistencePartie(FILE *bdd_parties, const char *nom_partie_a_verifier);
void creerPartie(FILE *bdd_parties);
PARTIE rechercherPartieParNom(FILE *bdd_parties, const char *nom_cherche);
void reprendrePartie();

int compterNombreParties(FILE *bdd_parties)
{
    PARTIE partie;
    int nombre_parties = 0;

    fseek(bdd_parties, 0, SEEK_SET);
    while (fread(&partie, sizeof(PARTIE), 1, bdd_parties))
    {
        nombre_parties = nombre_parties + 1;
    }

    return nombre_parties;
}

int verifierExistencePartie(FILE *bdd_parties, const char *nom_partie_a_verifier)
{
    PARTIE partie;
    fseek(bdd_parties, 0, SEEK_SET);
    while (fread(&partie, sizeof(PARTIE), 1, bdd_parties)) {
        if (strcmp(partie.nom, nom_partie_a_verifier) == 0) {
            return 1;
        }
    }

    return 0;
}

void creerPartie(FILE *bdd_parties)
{
    PARTIE partie;
    int nombre_parties = compterNombreParties(bdd_parties);
    int partie_existe = 0;

    printf("Entrez le nom de la partie : ");
    scanf("%s", partie.nom);

    partie_existe = verifierExistencePartie(bdd_parties, partie.nom);

    if (partie_existe) {
        printf("Ce nom de partie est déjà utilisé par une autre partie, veuillez en choisir un autre ou reprenez cette partie.\n");
    } else {
        partie.id = nombre_parties + 1;

        fseek(bdd_parties, 0, SEEK_END);
        if (fwrite(&partie, sizeof(PARTIE), 1, bdd_parties) != 1) {
            perror("Erreur lors de l'écriture dans le fichier");
        } else {
            printf("Les informations ont été enregistrées avec succès.\n");
        }
    }

    return;
}