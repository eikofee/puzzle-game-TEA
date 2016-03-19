#ifndef _PIECE_RH_
#define _PIECE_RH_

#include <piece.h>

/**
 * Permet la création d'une pièce Rush Hour
 * @param x La coordonnée sur l'axe X de la pièce.
 * @param y La coordonnée sur l'axe Y de la pièce.
 * @param small Le boolèen permettant de savoir si c'est un petit ou un grand véhicule.
 * @param horizontal Le booleen permettant de savoir si le véhicule est à l'horizontal ou à la vertical
 * @return La pièce initialisée avec les paramètres.
 */
piece new_piece_rh (int x, int y, bool small, bool horizontal);

/**
 * Permet de savoir l'orientation de la pièce en paramètre.
 * @param p La pièce qui va être testée.
 * @return Un boolèen, True si la pièce p est horizontal, False sinon.
 */
bool is_horizontal(cpiece p);
#endif