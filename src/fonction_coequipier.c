void coequipier(Personnage *p, const char *demande, const char *type) {
    if (strcmp(demande, "manger") == 0) {
        if (strcmp(type, "cuistot present") == 0) {
            if (p->nourriture >= 1 && p->faim > 0) {
                p->faim -= 5;
                if (p->faim < 0) p->faim = 0;
                p->nourriture--; // Prend 1 nourriture
                printf("%s a mangé avec le cuistot. Faim : %d, Nourriture restante : %d\n", p->nom, p->faim, p->nourriture);
            } else {
                printf("Pas assez de nourriture ou %s n'a pas faim.\n", p->nom);
            }
        } else if (strcmp(type, "cuistot absent") == 0) {
            if (p->nourriture >= 5) {
                p->faim -= 5;
                if (p->faim < 0) p->faim = 0;
                p->nourriture -= 5; // Prend 5 nourritures
                printf("Pas de cuistot. %s a mangé mais a consommé plus de nourriture. Faim : %d, Nourriture restante : %d\n", p->nom, p->faim, p->nourriture);
            } else {
                printf("Pas assez de nourriture pour %s.\n", p->nom);
            }
        }
    } else if (strcmp(demande, "soins") == 0) {
        if (strcmp(type, "soigneur present") == 0) {
            if (p->soin >= 1 && p->sante < 100) {
                p->sante += 5; // Soigne 5 PV
                if (p->sante > 100) p->sante = 100;
                p->soin--; // Prend 1 soin
                printf("%s a été soigné avec le soigneur. Santé : %d, Soins restants : %d\n", p->nom, p->sante, p->soin);
            } else {
                printf("Pas assez de soins ou %s est en pleine santé.\n", p->nom);
            }
        } else if (strcmp(type, "soigneur absent") == 0) {
            if (p->soin >= 2) {
                p->sante += 3; // Soigne 3 PV
                if (p->sante > 100) p->sante = 100;
                p->soin -= 2; // Prend 2 soins
                printf("Pas de soigneur. %s a été soigné partiellement. Santé : %d, Soins restants : %d\n", p->nom, p->sante, p->soin);
            } else {
                printf("Pas assez de soins pour %s.\n", p->nom);
            }
        }
    } else {
        printf("Demande invalide.\n");
    }
}
