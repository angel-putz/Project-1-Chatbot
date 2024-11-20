#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_EVENEMENTS 100
#define MAX_LONGUEUR_EVENEMENT 100

// Liste de synonymes pour déclencher le déplacement
const char *synonymes_deplacement[] = {"déplace", "avance", "marche", "bouge"};
const int nombre_synonymes = sizeof(synonymes_deplacement) / sizeof(synonymes_deplacement[0]);

// Fonction pour charger les événements à partir du fichier
int charger_evenements(char evenements[][MAX_LONGUEUR_EVENEMENT], const char *nom_fichier) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier %s.\n", nom_fichier);
        return 0;
    }
    int i = 0;
    while (fgets(evenements[i], MAX_LONGUEUR_EVENEMENT, fichier) != NULL && i < MAX_EVENEMENTS) {
        evenements[i][strcspn(evenements[i], "\n")] = '\0'; // Retire le saut de ligne
        i++;
    }
    fclose(fichier);
    return i; // Retourne le nombre d'événements chargés
}

// Fonction pour lancer un événement aléatoire
void lancer_evenement_aleatoire(char evenements[][MAX_LONGUEUR_EVENEMENT], int nombre_evenements) {
    if (nombre_evenements > 0) {
        int indice = rand() % nombre_evenements; // Choisit un événement au hasard
        printf("Événement aléatoire : %s\n", evenements[indice]);
    } else {
        printf("Aucun événement disponible.\n");
    }
}

// Fonction simulant le déplacement du joueur et déclenchant un événement
void deplacer_joueur(char evenements[][MAX_LONGUEUR_EVENEMENT], int nombre_evenements) {
    printf("Vous vous déplacez...\n");
    lancer_evenement_aleatoire(evenements, nombre_evenements);
}

// Fonction pour détecter si un mot-clé de déplacement est présent
int contient_mot_cle(const char *commande) {
    for (int i = 0; i < nombre_synonymes; i++) {
        if (strstr(commande, synonymes_deplacement[i]) != NULL) {
            return 1; // Retourne vrai si un mot-clé est trouvé
        }
    }
    return 0; // Retourne faux si aucun mot-clé n'est trouvé
}

int main() {
    srand(time(NULL)); // Initialiser le générateur de nombres aléatoires

    // Charger les événements depuis le fichier
    char evenements[MAX_EVENEMENTS][MAX_LONGUEUR_EVENEMENT];
    int nombre_evenements = charger_evenements(evenements, "evenements.txt");

    // Simulation de commandes
    char commande[100];
    printf("Entrez une commande : ");
    while (fgets(commande, sizeof(commande), stdin) != NULL) {
        // Retire le saut de ligne de la fin de commande
        commande[strcspn(commande, "\n")] = '\0';

        // Vérifie si un mot-clé de déplacement est présent dans la commande
        if (contient_mot_cle(commande)) {
            deplacer_joueur(evenements, nombre_evenements);
        } else {
            printf("Commande inconnue. Essayez un texte contenant un mot comme \"déplace\", \"avance\", \"marche\" ou \"bouge\".\n");
        }

        printf("Entrez une commande : ");
    }

    return 0;
}