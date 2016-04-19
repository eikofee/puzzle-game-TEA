#Projet de Programmation - Semestre 4
Groupe IN400A3

Projet rush-hour

Le projet consiste à développer le jeu Rush Hour en interface texte (console) et graphique plus tard, avec un solveur.

Composition du groupe :
HALNAUT Adrien (Echoffee) - ORDONEZ Romain (Apodeus) - MARTY Yoan (tlanvaldear)

##Compiler et exécuter les jeux

- En se plaçant dans le répertoire rush-hour, taper:
```
cmake .
```

- Suivant le jeu souhaité, taper:
    - Pour Rush Hour : 
    ```
    make rush-hour
    ```
    - Pour le jeu de l'Ane Rouge
    ```
    make klotski
    ```
- Pour lancer le jeu, en se plaçant dans le dossier `bin`, taper:
    - Pour Rush Hour
  ```
    ./rush-hour
  ```
    - Pour le jeu de l'Ane Rouge
  ```
    ./klotski
  ```
**Avant de faire un nouveau `make`, penser à exécuter la commande `make clean`.**<br>

**Ne pas se placer dans le répertoire `bin` avant de lancer les exécutables peut créer un fichier `config.ini` hors du répertoire du projet**

- La batterie de tests peut être lancée à partir de la commande `ctest` en étant dans le dossier racine du projet.

##Solveur
Il existe deux version du solveur, l'une exécutable et l'autre intégrée dans le jeu.

###Version Exécutable
La version exécutable n'affiche que le nombre de coups minimal pour résoudre un puzzle donné.

Elle se lance comme indiqué :
```
  ./solveur [a|r] <file>
```

`a|r` sélectionne quel jeu résoudre, `a` pour le Klotski et `r` pour Rush-Hour
`<file>` est le fichier du niveau à résoudre, suivant ce modèle :
>  first line : \<grid_width\> \<grid_height\> <br>
>  second line : \<number of pieces\> <br>
>  next lines (1 line per piece) : <br>
>  \<x\> \<y\> \<width\> \<height\> \<can_move_x\> \<can_move_y\>

Un exemple est disponible à [cette adresse](http://puu.sh/olBmr/504a56fc43.txt).

###Version in-game
La commande `solve` résoud et affiche les mouvements pour résoudre la situation actuelle du niveau.