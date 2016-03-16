#include <stdlib.h>
#include <stdio.h>
#include <game_ar.h>
#include <utility.h>
#include <interface_txt.h>

void loadTheGame(char* id_src)
{
	//On crée le game g à partir d'un id source
	printf("\nLoading Game ...\n");
	game g = NULL;//getGameFromId(id_src);
	printf("Done\n\n");

	//On malloc un id qu'on utilisera pour etre modifié pendant la partie
	char* id = (char*) malloc(sizeof(char) * 128);
	if(id == NULL)
		error("getIdFromGame(), probleme d'allocation memoire");

	//getIdFromGame(g, id);
	draw_interface(g, id);

	while (!game_over_ar(g))
	{
		printf("Enter the car's number you want to move :\n");
		input_player(g, id);
		draw_interface(g, id);
	}

	//getIdFromGame(g, id);

	printf("************\n*--- GG ---*\n************\n");

	free(id);
	delete_game(g);
}

int main(void)
{
	char* Game1 = "10n4x5p3w2h2x2y0p3w1h2x1y0p3w1h2x0y0p3w2h1x0y2p3w2h1x2y2p3w1h2x0y3p3w1h1x1y3p3w1h1x1y4p3w1h1x2y3p3w1h1x2y4";
	//game g = getGameFromIdAR("3n10x10p3w4h2x2y2p3w2h2x6y6p3w10h2x0y8");
	game g = getGameFromIdAR(Game1);
	char* s = (char*) malloc(sizeof(char) * 256);
	while (true)
	{
		getIdFromGameAR(g, s);
		draw_interface(g, s);
		input_player(g, s);
	}

	return 0;
}

