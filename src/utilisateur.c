#define TAILLE_PSEUDO 21
#define OUI 0
#define NON 1
#define NOM_FICHIER "utilisateurs.bin"
#define NOM_DOSSIER "bdd"

typedef struct
{
    int id;
    char pseudo[TAILLE_PSEUDO];
    int partieId;
}UTILISATEUR;

void afficherMenuUtilisateur();
void listerUtilisateurs(FILE *bdd_utilisateurs);
void afficherUtilisateur(UTILISATEUR *utilisateur);
void creerUtilisateur(FILE *bdd_utilisateurs);
void consulterUtilisateur(FILE *bdd_utilisateurs);
int verifierExistencePseudo(FILE *bdd_utilisateurs, const char *pseudo_a_verifier);
void modifierUtilisateur(FILE *bdd_utilisateurs);
void supprimerUtilisateur(FILE *bdd_utilisateurs);
UTILISATEUR rechercherUtilisateurParNom(FILE *bdd_utilisateurs, char *pseudo_cherche);
int compterNombreUtilisateurs(FILE *bdd_utilisateurs);

void afficherMenuUtilisateur()
{
    FILE *bdd_utilisateurs = NULL;
	bdd_utilisateurs = ouvrirFichierDansDossier(NOM_DOSSIER, NOM_FICHIER);

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
        getchar(); // Vider le tampon d'entrée pour enlever le caractère '\n'

        switch (choix)
        {
            case 'R':
                printf("\nRetour au menu admin.\n");
                break;
            case 'l':
            case 'L':
                printf("\nLister les utilisateurs\n");
                listerUtilisateurs(bdd_utilisateurs);
                break;
            case 'c':
            case 'C':
                printf("\nConsulter les informations d'un utilisateur\n");
                consulterUtilisateur(bdd_utilisateurs);
                break;
            case 'a':
            case 'A':
                printf("\nAjouter un utilisateur\n");
                creerUtilisateur(bdd_utilisateurs);
                break;
            case 'm':
            case 'M':
                printf("\nModifier un utilisateur\n");
                modifierUtilisateur(bdd_utilisateurs);
                break;
            case 's':
            case 'S':
                printf("\nSupprimer un utilisateur\n");
                supprimerUtilisateur(bdd_utilisateurs);
                break;
            default:
                printf("\nChoix invalide, assurez-vous de saisir un caractère valide\n");
                break;
        }
    }

    fermerFichier(bdd_utilisateurs);
    return;
}

void listerUtilisateurs(FILE *bdd_utilisateurs) {
    printf("\nListe des utilisateurs:\n");

    UTILISATEUR utilisateur;
    int utilisateurTrouve = 0; // Indicateur pour savoir si au moins un utilisateur a été trouvé

    fseek(bdd_utilisateurs, 0, SEEK_SET);
    while (fread(&utilisateur, sizeof(UTILISATEUR), 1, bdd_utilisateurs)) {
        afficherUtilisateur(&utilisateur);
        utilisateurTrouve = 1; // On a trouvé au moins un utilisateur
    }

    if (!utilisateurTrouve) {
        printf("Il n'y a aucun utilisateur enregistré pour l'instant.\n");
    }
}

void afficherUtilisateur(UTILISATEUR *utilisateur) {
    printf("\nInformations de l'utilisateur :\n");
    printf("  - ID : %d\n", utilisateur->id);
    printf("  - Pseudo : %s\n", utilisateur->pseudo);
}

void creerUtilisateur(FILE *bdd_utilisateurs) {
    UTILISATEUR utilisateur;
    int nombre_utilisateurs = compterNombreUtilisateurs(bdd_utilisateurs);
    int pseudoExiste = 0;

    // Saisie des informations de la personne
    printf("Entrez votre pseudo : ");
    scanf("%s", utilisateur.pseudo);

    // Vérifier si le pseudo existe déjà
    pseudoExiste = verifierExistencePseudo(bdd_utilisateurs, utilisateur.pseudo);

    if (pseudoExiste) {
        printf("Ce pseudo est déjà utilisé par un autre utilisateur, veuillez en choisir un autre.\n");
    } else {
        // Initialisation des valeurs par défaut
        utilisateur.id = nombre_utilisateurs + 1;

        // Écriture de la personne dans le fichier
        fseek(bdd_utilisateurs, 0, SEEK_END);
        if (fwrite(&utilisateur, sizeof(UTILISATEUR), 1, bdd_utilisateurs) != 1) {
            perror("Erreur lors de l'écriture dans le fichier");
        } else {
            printf("Les informations ont été enregistrées avec succès.\n");
        }
    }
}

void consulterUtilisateur(FILE *bdd_utilisateurs) {
    char pseudo_cherche[TAILLE_PSEUDO];
    UTILISATEUR utilisateur;

    printf("Entrez le pseudo de l'utilisateur à consulter : ");
    scanf("%s", pseudo_cherche);

    utilisateur = rechercherUtilisateurParNom(bdd_utilisateurs, pseudo_cherche);

    if (utilisateur.id != 0) {
        afficherUtilisateur(&utilisateur);
    } else {
        printf("Utilisateur non trouvé.\n");
    }
}

int verifierExistencePseudo(FILE *bdd_utilisateurs, const char *pseudo_a_verifier) {
    UTILISATEUR utilisateur;
    fseek(bdd_utilisateurs, 0, SEEK_SET);
    while (fread(&utilisateur, sizeof(UTILISATEUR), 1, bdd_utilisateurs)) {
        if (strcmp(utilisateur.pseudo, pseudo_a_verifier) == 0) {
            return 1; // Le pseudo existe déjà
        }
    }
    return 0; // Le pseudo n'existe pas
}

void modifierUtilisateur(FILE *bdd_utilisateurs) {
    char pseudo_cherche[TAILLE_PSEUDO];
    UTILISATEUR utilisateur_a_modifier;

    printf("Entrez le pseudo de l'utilisateur à modifier : ");
    scanf("%s", pseudo_cherche);

    utilisateur_a_modifier = rechercherUtilisateurParNom(bdd_utilisateurs, pseudo_cherche);

    if (utilisateur_a_modifier.id != 0) { // Utilisateur trouvé
        // Affichage des informations actuelles de l'utilisateur
        afficherUtilisateur(&utilisateur_a_modifier);

        // Demander les modifications
        printf("Entrez le nouveau pseudo (laissez vide pour ne pas modifier) : ");
        scanf("%s", pseudo_cherche);
        if (strlen(pseudo_cherche) > 0) {
            strcpy(utilisateur_a_modifier.pseudo, pseudo_cherche);
        }

        // Réécrire l'utilisateur modifié dans le fichier
        fseek(bdd_utilisateurs, -sizeof(UTILISATEUR), SEEK_CUR); // Repositionner le curseur
        fwrite(&utilisateur_a_modifier, sizeof(UTILISATEUR), 1, bdd_utilisateurs);
        printf("Utilisateur modifié avec succès.\n");
    } else {
        printf("Utilisateur non trouvé.\n");
    }
}

void supprimerUtilisateur(FILE *bdd_utilisateurs) {
    char pseudo_cherche[TAILLE_PSEUDO];
    UTILISATEUR utilisateur_a_supprimer;

    printf("Entrez le pseudo de l'utilisateur à supprimer : ");
    scanf("%s", pseudo_cherche);

    utilisateur_a_supprimer = rechercherUtilisateurParNom(bdd_utilisateurs, pseudo_cherche);

    if (utilisateur_a_supprimer.id != 0) { // Utilisateur trouvé
        // Supprimer l'utilisateur en écrasant avec le prochain utilisateur
        UTILISATEUR utilisateur_suivant;
        while (fread(&utilisateur_suivant, sizeof(UTILISATEUR), 1, bdd_utilisateurs)) {
            fseek(bdd_utilisateurs, -2 * sizeof(UTILISATEUR), SEEK_CUR); // Repositionner le curseur
            fwrite(&utilisateur_suivant, sizeof(UTILISATEUR), 1, bdd_utilisateurs);
        }
        if (ftruncate(fileno(bdd_utilisateurs), ftell(bdd_utilisateurs) - sizeof(UTILISATEUR)) != 0) {
            perror("Erreur lors du tronquage du fichier");
        } else {
            printf("Utilisateur supprimé avec succès.\n");
        }
    } else {
        printf("Utilisateur non trouvé.\n");
    }
}

UTILISATEUR rechercherUtilisateurParNom(FILE *bdd_utilisateurs, char *pseudo_cherche)
{
    UTILISATEUR utilisateur;

    fseek(bdd_utilisateurs, 0, SEEK_SET);
    while (fread(&utilisateur, sizeof(UTILISATEUR), 1, bdd_utilisateurs))
    {
        if (strcmp(utilisateur.pseudo, pseudo_cherche) == 0)
        {
            return utilisateur; // Retourner la personne trouvée
        }
    }

    printf("Personne avec le nom '%s' non trouvée dans le fichier.\n", pseudo_cherche);
    UTILISATEUR utilisateurNonTrouvee;
    memset(&utilisateurNonTrouvee, 0, sizeof(UTILISATEUR));
    return utilisateurNonTrouvee;
}

int compterNombreUtilisateurs(FILE *bdd_utilisateurs)
{
    UTILISATEUR utilisateur;
    int nombre_utilisateurs = 0;

    fseek(bdd_utilisateurs, 0, SEEK_SET);
    while (fread(&utilisateur, sizeof(UTILISATEUR), 1, bdd_utilisateurs))
    {
        nombre_utilisateurs = nombre_utilisateurs + 1;
    }

    return nombre_utilisateurs;
}