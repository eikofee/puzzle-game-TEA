#ifndef _GAME_RH_H_
#define _GAME_RH_H_


#include <game.h>
#include <piece_rh.h>
#include <function_interface.h>

/**
 * Permet de créer une structure game avec les paramètres spécifiques au jeu Rush Hour
 * @param nb_pieces Le nombre de pièce se trouvant dans le tableau de pièces.
 * @param pieces Un tableau de pièce.
 * @return un pointeur *game initialisé.
 */
game new_game_hr( int nb_pieces, piece *pieces);

/**
 * Permet de tester l'etat du jeu passé en paramètre, savoir si il est fini ou non.
 * @param g Le jeu qui va être testé.
 * @return Un Booleen, True si le jeu est fini, False sinon.
 */
bool game_over_hr(cgame g);

#endif