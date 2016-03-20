### Projet de Programmation - Semestre 4 ###
Groupe IN400A3

Projet rush-hour (Version interface Texte)

Le projet consiste à développer le jeu Rush Hour en interface texte (console) et graphique plus tard, avec un solveur.

Composition du groupe :
HALNAUT Adrien (Echoffee) - ORDONEZ Romain (Apodeus) - MARTY Yoan (tlanvaldear)


### Compiler et exécuter les jeux ###

- En se plaçant dans le répertoire rush-hour, taper:

$> cmake .

- Suivant le jeu souhaité, taper:
    - Pour Rush Hour : 
    
    $> make rush-hour
    
    - Pour le jeu de l'Ane Rouge
    
    $> make klotski
    
- Pour lancer le jeu, en se plaçant dans le dossier `bin`, taper:
    - Pour Rush Hour
  
    $> ./rush-hour
  
    - Pour le jeu de l'Ane Rouge
  
    $> ./klotski
  
*** Avant de faire un nouveau `make`, penser à exécuter la commande `make clean`. ***

*** Ne pas se placer dans le répertoire `bin` avant de lancer les exécutables peut créer un fichier `config.ini` hors du répertoire du projet ***

- La batterie de tests peut être lancée à partir de la commande `ctest` en étant dans le dossier racine du projet.