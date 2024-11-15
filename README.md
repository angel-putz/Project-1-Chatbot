# Projet Chat-Bot

## Collaborateurs

- [Angel Putz](https://github.com/angel-putz)
- [Corentin BOISSIE](https://github.com/Ramen2Foutre)
- [Clément FAVAREL](https://github.com/clementfavarel)

## Description du projet

### Sujet

Vous devez créer un jeu textuel interactif, inspiré par la mécanique d'un chat-bot, où le joueur interagit via des commandes textuelles.

### Thème

**APOCALYPSE : Survive or Die**

**Description :**

Un jeu où le joueur doit survivre dans un monde post-apocalyptique infesté de zombies. En tant que chef de groupe, il gère les ressources, prend des décisions cruciales, et doit interagir avec des survivants pour maintenir leur moral.

**Fonctionnalités :**

1. Création du Personnage :
   Background : Inventer une histoire générique pour la création de n'importe quel PP.
   Traits de Personnalité Évolutifs : Introduire des traits de personnalité qui peuvent changer en fonction des décisions du joueur, ajoutant une dimension dynamique au personnage.

2. Création des PNJs :
   Génération Dynamique : Intégrer un système de génération aléatoire qui permet de créer des PNJs avec des caractéristiques.
   Profils Comportementaux : Établir des intéractions qui peuvent faire gagner/perdre de la santé, du moral.

3. Attributs des PNJs :
   Attributs Multiples : Ajouter des attributs comme le Charisme (influence sur le moral des autres), le Leadership (capacité à gérer un groupe) ou la Résilience (capacité à supporter des événements traumatisants). (OPTIONNEL)
   Interactions Sociales : Créer un système où les attributs influencent les dialogues et les décisions, permettant aux PNJs d'agir différemment selon leurs caractéristiques. (si son moral, charisme est élévé cela génerera une réponse/action différente par ex)

4. Gestion des Ressources :
   Système de Production : Introduire des mécanismes de production où les joueurs peuvent créer ou cultiver des ressources (cultiver des plantes médicinales, par exemple).
   Conséquences des Choix : Élargir l'impact des choix sur la gestion des ressources, par exemple en ajoutant des événements qui réduisent les stocks (p. ex., un groupe de survivants qui vole des provisions).

5. Décisions Épiques : (OPTIONNEL)
   Système de Dilemmes : Mettre en place des dilemmes plus nuancés où les conséquences des décisions ne sont pas immédiatement visibles, rendant les choix plus complexes.
   Impact à Long Terme : Les décisions pourraient avoir des répercussions sur l'intrigue globale ou sur la disponibilité de certains PNJs dans le futur.

6. Événements Aléatoires : (ELEMENT ALEATOIRE)
   Événements Narratifs : Introduire des événements scénarisés qui approfondissent l'histoire du monde (par exemple, un camp de survivants en détresse, une rumeur sur un refuge sûr).
   Variété des Événements : Créer une liste d'événements variés (certaines positifs, d'autres négatifs) qui affectent l’environnement et les stocks, ajoutant une dimension d'imprévisibilité.

7. Score de Moralité :
   Moralité Nuancée : Introduire des sous-catégories de moralité (ex. : altruisme, égoïsme, courage, peur) qui pourraient influencer les interactions et les relations.
   Répercussions Psychologiques : Ajouter un mécanisme où le score de moralité influence la santé mentale du personnage principal, entraînant des effets secondaires (stress, dépression) selon les choix faits.

8. Menu Admin :
   Ce dernier pourra par exemple créer un nouveau PNJ, le modifier ou le supprimer.

### Description technique

Pour consulter la description technique, référez-vous à la documentation : `./docs/P22_Projet_FAVAREL_PUTZ_BOISSIE.docx`

## Instructions de build

A la racine du projet, executez les commande :

```bash
mkdir build
gcc src/main.c -o build/main
```
