#ifndef _INTERFACE_SDL_H_
#define _INTERFACE_SDL_H_

#include <game.h>
#include <game_rh.h>
#include <game_ar.h>

bool game_over(game g);
// #include <interface_txt.h>
/**
 * Fonction qui effectue le mouvement en passant par une copie (p_copy)
 * @param p La pièce qui va subir le déplacement.
 * @param d La direction dans laquelle on veut déplacer la pièce p.
 * @param distance La distance sur laquelle on veut effectuer le déplacement.
 */
int choixDuJeu();

/**
 * Crée un tableau 2D correspondant au plateau du jeu. Les cases occupées par les pièces sont remplies
 * par l'indice de la pièce qui occupe la case. -1 sinon.
 * @param array_p Un tableau de pièce.
 * @param size La taille du tableau de pièce.
 * @param size_x La largeur du plateau ( en fonction du jeu )
 * @param size_y La hauteur du plateau
 * @return Un tableau 2D d'entiers.
 */
void init_sdl_game(game g, int *continuer, int indGame);



#endif