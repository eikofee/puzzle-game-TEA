#ifndef _INTERFACE_SDL_H_
#define _INTERFACE_SDL_H_

#include <game.h>
#include <game_rh.h>
#include <game_ar.h>
#include <interface_txt.h>

/**
 * Permet l'affichage de la fenetre de selection de jeux sous sdl.
 * @return un entier pour savoir comment s'est executé la fonction. retourn -1 si l'utilisateur décide de quitter.
 */
int choixDuJeu();

/**
 * Affiche l'interface du jeu avec la grille du jeu, et le nombre de mouvement.
 * @param g Le jeu qui doit être affiché
 * @param *continuer un pointeur sur un entier, continuer représente la condition d'arret de la boucle qui agit sur la fonction.
 * @param indGame Un entier qui permet de savoir quel niveau lire dans les fichiers .txt contenant les IDs.
 */
void init_sdl_game(game g, int *continuer, int indGame);



#endif