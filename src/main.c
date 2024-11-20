/*
@author = clementfavarel
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdbool.h>
#include "jeu.c"
#include "fichier.c"
#include "utilisateur.c"
#include "partie.c"
#include "admin.c"

int main(int argc, char **argv)
{
    afficherTexteIntro();
    afficherMenuPrincipal();

    return 0;
}