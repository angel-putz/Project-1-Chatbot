/*
@author = clementfavarel
*/

#ifdef _WIN32
#include <direct.h>
#define MKDIR(dossier) _mkdir(dossier)
#else
#include <unistd.h>
#define MKDIR(dossier) mkdir(dossier, 0777)
#endif

FILE *ouvrirFichier(char *nom_fichier);
void fermerFichier(FILE *fichier_ouvert);
FILE *ouvrirFichierDansDossier(char *nom_dossier, char *nom_fichier);

FILE *ouvrirFichier(char *nom_fichier)
{
    FILE *fichier = NULL;
    fichier = fopen(nom_fichier, "a");
    fclose(fichier);
    fichier = fopen(nom_fichier, "r+");
    return fichier;
}

void fermerFichier(FILE *fichier_ouvert)
{
    fclose(fichier_ouvert);
    return;
}

FILE *ouvrirFichierDansDossier(char *nom_dossier, char *nom_fichier)
{
    FILE *fichier = NULL;
    char chemin_complet[256];

    snprintf(chemin_complet, sizeof(chemin_complet), "%s/%s", nom_dossier, nom_fichier);

    fichier = fopen(chemin_complet, "a");

    if (fichier == NULL)
    {
        if (MKDIR(nom_dossier) != 0)
        {
            perror("Erreur lors de la création du dossier");
            return NULL;
        }

        fichier = fopen(chemin_complet, "a");
        if (fichier == NULL)
        {
            perror("Erreur lors de l'ouverture ou de la création du fichier");
            return NULL;
        }
    }

    fclose(fichier);

    fichier = fopen(chemin_complet, "r+");
    if (fichier == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier en mode r+");
    }

    return fichier;
}