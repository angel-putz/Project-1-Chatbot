/*
@author = clementfavarel
*/

// Fonctions du fichier
void commencerAventure(PARTIE *partie);

void commencerAventure(PARTIE *partie) {
    if (partie == NULL) {
        printf("Erreur : Partie non initialisée.\n");
        return;
    }

    printf("--- Début de la partie : %s ---\n", partie->nom);
    printf("Tapez 'Q' puis 'Entrée' pour quitter la partie.\n");

    char choix;
    while (1) {
        choix = getchar(); // Capture l'entrée utilisateur
        
        if ((choix == 'Q') || (choix == 'q')) {
            printf("Vous avez quitté la partie.\n");
            break;
        }
    }
}
