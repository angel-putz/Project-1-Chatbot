
void infligerDegats(void *cible, int degats, char type) {
    if (type == 'P') { // Si la cible est un personnage
        PERSONNAGE *perso = (PERSONNAGE *)cible;
        if (perso->pv > 0) {
            perso->pv -= degats; // R�duction des points de vie
            if (perso->pv < 0) {
                perso->pv = 0; // Les points de vie ne peuvent pas �tre n�gatifs
            }
            printf("%s a subi %d d�g�ts. Points de vie restants : %d\n", perso->nom, degats, perso->pv);
        } else {
            printf("%s est d�j� hors de combat.\n", perso->nom);
        }
    } else if (type == 'Z') { // Si la cible est un zombie
        ZOMBIE *zombie = (ZOMBIE *)cible;
        if (zombie->pv > 0) {
            zombie->pv -= degats; // R�duction des points de vie
            if (zombie->pv < 0) {
                zombie->pv = 0; // Les points de vie ne peuvent pas �tre n�gatifs
            }
            printf("Le zombie a subi %d d�g�ts. Points de vie restants : %d\n", degats, zombie->pv);
        } else {
            printf("Le zombie est d�j� hors de combat.\n");
        }
    }
}
