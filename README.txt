### Projet de Programmation - Semestre 4 ###
Groupe IN400A3

Projet rush-hour / Ane Rouge

Le projet consiste à développer le jeu Rush Hour et l'Ane Rouge en interface texte (console) et graphique, avec un solveur.

Composition du groupe :
HALNAUT Adrien (Echoffee) - ORDONEZ Romain (Apodeus) - MARTY Yoan (tlanvaldear)


### Compiler et exécuter les jeux ###
- Pour compiler la version Graphique, assurez vous d'avoir installé SDL en version 1.2, SDL_ttf, SDL_Image et SDL_Mixer.

- En se plaçant dans le répertoire racine 'Puzzle-game-TEA', taper:

$> cmake .

- Suivant le jeu souhaité, taper:
    - Pour Rush Hour : 
    
    $> make rush-hour
    
    - Pour le jeu de l'Ane Rouge
    
    $> make klotski

    - Pour obtenir les deux jeux avec une interface graphique :

    $> make interfaceGraphique

    - Si vous voulez compiler tout les executables, taper:

    $> make
    
- Pour lancer le jeu, en se plaçant dans le dossier `bin`, taper:
    - Pour Rush Hour
  
    $> ./rush-hour
  
    - Pour le jeu de l'Ane Rouge
  
    $> ./klotski

    - Pour l'interface graphique

    $> ./interfaceGraphique
  
*** Avant de faire un nouveau `make`, penser à exécuter la commande `make clean`. ***

*** Ne pas se placer dans le répertoire `bin` avant de lancer les exécutables peut créer un fichier `config.ini` hors du répertoire du projet ***

- La batterie de tests peut être lancée à partir de la commande `ctest` en étant dans le dossier racine du projet.

### Solveur ###

Il existe deux version du solveur, l'une exécutable et l'autre intégrée dans le jeu.

## Version Exécutable ##

La version exécutable n'affiche que le nombre de coups minimal pour résoudre un puzzle donné.

Elle se lance comme indiqué :

  $> ./solveur [a|r] <file>

a|r sélectionne quel jeu résoudre, a pour le Klotski et r pour Rush-Hour
<file> est le fichier du niveau à résoudre, suivant ce modèle :

first line : <grid_width> <grid_height> 
second line : <number of pieces> 
next lines (1 line per piece) : 
<x> <y> <width> <height> <can_move_x> <can_move_y>
Un exemple est disponible à cette adresse : http://puu.sh/olBmr/504a56fc43.txt 

## Version in-game ##

La commande 'solve' résoud et affiche les mouvements pour résoudre la situation actuelle du niveau.

## Interface Graphique ##

Pour choisir votre jeu, un simple clic sur le nom du jeu de votre choix.

Une fois en-jeu, passez votre souris sur le rectangle 'Help' pour avoir davantages d'informations concernant
le but du jeu, et comment y jouer.

Le bouton Reset permet de réinitialiser la partie actuelle.

Appuyer sur Echap pour afficher le menu Echap afin de quitter le programme.

La police utilisée pour l'interface graphique est la police sansation-regular.ttf 
