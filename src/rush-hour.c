#include <stdlib.h>
#include <stdio.h>
#include "game.h"
#include "utility.h"
#include "interface_txt.h"


int main(int argc, char* argv[])
{
	//game g = test_level();
	//display_pieces(g -> pieces, g -> nb_pieces);
	game g = getGameFromId(73002220527);
	display_pieces(g -> pieces, g -> nb_pieces);
	printf("Game done\n");
	while (!game_over_hr(g))
	{
		draw_interface(g, getIdFromGame(g));
		input_player(g);
	}
	draw_interface(g, getIdFromGame(g));
	printf("************\n*--- GG ---*\n************\n");
	return 0;
}
