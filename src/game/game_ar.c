#include <stdio.h>
#include <stdlib.h>
#include <game.h>
#include <game_ar.h>
#include <utility.h>

game new_game_ar( int nb_pieces, piece *pieces)
{
	game newGame_ar = new_game(4, 5, nb_pieces, pieces);
	if(newGame_ar == NULL)
		error("new_game_hr(), porbleme d allocation memoire");
	return newGame_ar;
}

bool game_over_ar(cgame g)
{
	return (get_x(game_piece(g, 0)) == 1 && get_y(game_piece(g, 0)) == 0);
}