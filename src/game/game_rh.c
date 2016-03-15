#include <stdio.h>
#include <stdlib.h>
#include <game.h>
#include <game_rh.h>
#include <utility.h>

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
	int x_voiture = get_x(game_piece(g, 0));
	return (x_voiture == (game_width(g) - get_width(game_piece(g, 0))));
}
