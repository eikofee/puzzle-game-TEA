#include <stdlib.h>
#include <stdio.h>
#include "game.h"

struct game_s{
	piece *pieces;	//malloc nécessaire
	int	nb_moves;
}

game new_game_hr( int nb_pieces, piece *pieces)
{
	game new_game = (game_s) malloc(sizeof(struct game_s));
	new_game -> pieces = (piece) malloc(sizeof(struct piece_s) * nb_pieces);

	for (int i = 0; i < nb_pieces; i++)
	{
		new_game -> pieces[i] = pieces[i];
	}

	new_game -> nb_moves = 0;
	return new_game;
}