#include <stdio.h>
#include <stdlib.h>
#include <game.h>
#include <game_rh.h>
#include <utility.h>

//Le jeu Rush Hour classique se joue sur un plateau 6x6
#define TAILLE_PLATEAU_RH 6

game new_game_hr( int nb_pieces, piece *pieces)
{
	game newGame_rh = new_game(TAILLE_PLATEAU_RH, TAILLE_PLATEAU_RH, nb_pieces, pieces);
	if(newGame_rh == NULL)
		error("new_game_hr(), porbleme d allocation memoire");
	return newGame_rh;
}


bool game_over_hr(cgame g)
{
	//On pourrait tout écrire sur une ligne, mais juste pour des soucis de lisibilité et de compréhension
	// on fait le test en deux temps.
	// On regarde la position en X de la voiture principal d'indice 0
	int x_voiture = get_x(game_piece(g, 0));

	//Et on test si elle est égale à la taille du plateau en X moins la taille du véhicule.
	return (x_voiture == (game_width(g) - get_width(game_piece(g, 0))));
}
