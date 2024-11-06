// Lib for booleans
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Variables globales à utiliser avec ces structures
#define OUI 0
#define NON 1
#define TAILLE_NOM 51
#define TAILLE_MESSAGE 1001
#define TAILLE_COMPETENCE 101
#define MAX_PARTIES 3

// Personnages Non Jouables (PNJ)
typedef struct {
    char nom[TAILLE_NOM];
    char message[TAILLE_MESSAGE];
    bool mort;
} PNJ;

// Personnages de l'histoire (joueur principal et coéquipiers)
typedef struct {
    char nom[TAILLE_NOM];
    int pv;
    int faim;
    bool sante;
    char competence[TAILLE_COMPETENCE];
} PERSONNAGE;

// Zombies
typedef struct {
    int pv;
    int degat;
    bool mort;
} ZOMBIE;

// LOGIQUE PARTIES - UTILISATEURS
// Parties
typedef struct
{
    int id;
    char nom[TAILLE_NOM];
    int id_utilisateur;
}PARTIE;

typedef struct
{
    int id;
    char nom[TAILLE_NOM];
    int parties[MAX_PARTIES];
    int nombre_parties;
}UTILISATEUR;

// Création de parties
int nombre_parties = 0;
PARTIE partie;
UTILISATEUR utilisateur;
parties[nombre_parties++] = (PARTIE){1, "Partie de machin", utilisateur[0].id};
