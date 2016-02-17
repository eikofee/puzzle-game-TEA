#Projet de Programmation - Semestre 4
Groupe IN400A3

Projet rush-hour

Le projet consiste à développer le jeu Rush Hour en interface texte (console) et graphique plus tard, avec un solveur.

Composition du groupe :
HALNAUT Adrien (Echoffee) - ORDONEZ Romain (Apodeus) - MARTY Yoan (tlanvaldear)

##Faire un push à partir d'un pull obsolète
Il faut relocaliser notre sous-branche à la pointe la plus récente,
on utilisera donc `git pull --rebase`. On aura donc:

```
$> git push origin master
[[message d'erreur à propos du fast forward]]

$> git pull --rebase
[[...Premièrement, rembobinons head pour rejouer votre travail par-dessus...]]

$> git push origin master
[[Tout est ok]]
```