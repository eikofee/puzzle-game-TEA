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

##Faire un push à partir d'un pull obsolète ET conflit lors du `pull --rebase`
`git mergetool` semble très utile pour gérer les conflits.
Penser à lancer `git rebase --continue` après la résolution. On aura donc:
```
$> git push origin master
[[Message d'erreur fast forward]]

$> git pull --rebase
[[Message de conflit]]

$> git mergetool					//marche aussi sans fichier en paramètre
[[Résolution de votre côté, les deux fichiers sont dans la partie supérieure,
 le nouveau fichier se trouve dans la partie inférieur, appliquer les transformations 
 en faisant un clique droit sur les parties en conflit]]

$> git rebase --continue
[[Succès]]

$> git push origin master
[[Tout est ok]]
```

##Afficher l'historique des commits et les modifier/supprimer
Il faut que l'éditeur de texte de git soit **correctement** configuré.
```
$> git rebase -i HEAD~n
```
Cette commande ouvre l'éditeur avec les n derniers commits enregistrés, locaux comme push'd.
Les instructions pour les modifiers sont affichées en commentaire, il suffit de modifier les 
lignes de commit, d'enregistrer et de fermer l'éditeur, et des instructions seront affichées
dans la console pour appliquer les changements souhaités. Useful as hell.