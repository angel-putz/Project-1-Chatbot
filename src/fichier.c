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

FILE *ouvrirFichier(char *nomFichier);
void fermerFichier(FILE *fichierOuvert);
FILE *ouvrirFichierDansDossier(char *nomDossier, char *nomFichier);

FILE *ouvrirFichier(char *nomFichier)
{
    FILE *fichier = NULL;
    fichier = fopen(nomFichier, "a");
    fclose(fichier);
    fichier = fopen(nomFichier, "r+");
    return fichier;
}

void fermerFichier(FILE *fichierOuvert)
{
    fclose(fichierOuvert);
    return;
}

FILE *ouvrirFichierDansDossier(char *nomDossier, char *nomFichier)
{
    FILE *fichier = NULL;
    char cheminComplet[256];

    snprintf(cheminComplet, sizeof(cheminComplet), "%s/%s", nomDossier, nomFichier);

    fichier = fopen(cheminComplet, "a");

    if (fichier == NULL)
    {
        if (MKDIR(nomDossier) != 0)
        {
            perror("Erreur lors de la création du dossier");
            return NULL;
        }

        fichier = fopen(cheminComplet, "a");
        if (fichier == NULL)
        {
            perror("Erreur lors de l'ouverture ou de la création du fichier");
            return NULL;
        }
    }

    fclose(fichier);

    fichier = fopen(cheminComplet, "r+");
    if (fichier == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier en mode r+");
    }

    return fichier;
}