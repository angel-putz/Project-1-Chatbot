#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/stat.h>
#include "jeu.c"
#include "admin.c"
#include "fichier.c"
#include "utilisateur.c"

int main(int argc, char **argv) {
    afficherTexteIntro();
    afficherMenuPrincipal();

    return 0;
}