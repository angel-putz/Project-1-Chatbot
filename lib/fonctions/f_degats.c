
void infligerDegats(void *cible, int degats, char type) {
    if (type == 'P') { // Si la cible est un personnage
        PERSONNAGE *perso = (PERSONNAGE *)cible;
        if (perso->pv > 0) {
            perso->pv -= degats; // Réduction des points de vie
            if (perso->pv < 0) {
                perso->pv = 0; // Les points de vie ne peuvent pas être négatifs
            }
            printf("%s a subi %d dégâts. Points de vie restants : %d\n", perso->nom, degats, perso->pv);
        } else {
            printf("%s est déjà hors de combat.\n", perso->nom);
        }
    } else if (type == 'Z') { // Si la cible est un zombie
        ZOMBIE *zombie = (ZOMBIE *)cible;
        if (zombie->pv > 0) {
            zombie->pv -= degats; // Réduction des points de vie
            if (zombie->pv < 0) {
                zombie->pv = 0; // Les points de vie ne peuvent pas être négatifs
            }
            printf("Le zombie a subi %d dégâts. Points de vie restants : %d\n", degats, zombie->pv);
        } else {
            printf("Le zombie est déjà hors de combat.\n");
        }
    }
}
