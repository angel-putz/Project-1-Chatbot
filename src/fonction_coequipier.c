void coequipier(Personnage *p, const char *demande, const char *type) {
    if (strcmp(demande, "manger") == 0) {
        if (strcmp(type, "cuistot present") == 0) {
            if (p->nourriture > 0 && p->faim > 0) {
                p->faim -= 5; // Diminue la faim
                if (p->faim < 0) p->faim = 0;
                p->nourriture--; // Consomme une unité de nourriture
                printf("%s a mangé grâce au cuistot. Faim : %d, Nourriture restante : %d\n", p->nom, p->faim, p->nourriture);
            } else {
                printf("Pas assez de nourriture ou %s n'a pas faim.\n", p->nom);
            }
        } else if (strcmp(type, "cuistot absent") == 0) {
            printf("Pas de cuistot disponible. %s ne peut pas manger pour le moment.\n", p->nom);
        }
    }
    else if (strcmp(demande, "soins") == 0) {
        if (strcmp(type, "soigneur present") == 0) {
            if (p->soin > 0 && p->sante < 100) {
                p->sante += 20; // Augmente la santé
                if (p->sante > 100) p->sante = 100;
                p->soin--; // Consomme une unité de soin
                printf("%s a été soigné grâce au soigneur. Santé : %d, Soins restants : %d\n", p->nom, p->sante, p->soin);
            } else {
                printf("Pas assez de soins ou %s est en pleine santé.\n", p->nom);
            }
        } else if (strcmp(type, "soigneur absent") == 0) {
            printf("Pas de soigneur disponible. %s ne peut pas être soigné pour le moment.\n", p->nom);
        }
    } else {
        printf("Demande invalide.\n");
    }
}
