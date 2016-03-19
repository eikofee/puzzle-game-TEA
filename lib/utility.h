#ifndef _UTILITY_H_
#define _UTILITY_H_

/**
 * Fonction qui effectue le mouvement en passant par une copie (p_copy)
 * @param p La pièce qui va subir le déplacement.
 * @param d La direction dans laquelle on veut déplacer la pièce p.
 * @param distance La distance sur laquelle on veut effectuer le déplacement.
 */
void switchsMovePiece(piece p, dir d, int distance);

/**
 * Crée un tableau 2D correspondant au plateau du jeu. Les cases occupées par les pièces sont remplies
 * par l'indice de la pièce qui occupe la case. -1 sinon.
 * @param tab_pieces Un tableau de pièce.
 * @param taille La taille du tableau de pièce.
 * @param taille_x La largeur du plateau ( en fonction du jeu )
 * @param taille_y La hauteur du plateau
 * @return Un tableau 2D d'entiers.
 */
int** mapPieces(piece* tab_pieces,int taille, int taille_x, int taille_y);

/**
 * Crée un tableau 2D d'entier qui correspond à toutes les coordonnées des cases occupées par la pièce p
 * @param p La pièce dont on veut obtenir toutes ses coordonnées
 * @param taille Le nombre de case qu'occupe la piece p. C'est la surface : (largeur x hauteur)
 * @return Le tableau 2D d'entier qui correspond à toutes les coordonnées des cases occupées par la piece p.
 */
int** pieceArray(piece p, int taille);

/**
 * Permet d'ecrire un message dans la sortie standard d'erreur, et d'arreter le programme.
 * @param s Le message que l'on veut ecrire sur stderr.
 */
void error(char* s);

#endif