#ifndef _GAME_AR_H_
#define _GAME_AR_H_

#include <game.h>
#include <function_interface.h>

/**
 * Permet de créer une structure game avec les paramètres spécifiques au jeu de l'Ane Rouge ( Klotski )
 * @param nb_pieces Le nombre de pièce se trouvant dans le tableau de pièces.
 * @param pieces Un tableau de pièce.
 * @return un pointeur *game initialisé.
 */
game new_game_ar( int nb_pieces, piece *pieces);

/**
 * Permet de savoir l'état du jeu, si il est fini ou non, en regardant la position de la voiture 0  
 * @param g Correspond au jeu qui doit être testé.
 * @return Un booleen, True si le jeu est fini, False sinon.
*/
bool game_over_ar(cgame g);

#endif