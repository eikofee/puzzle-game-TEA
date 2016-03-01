#include <stdlib.h>
#include <stdio.h>
#include "game.h"
#include "utility.h"
#include "interface_txt.h"

//Fonction qui permet le chargement et l'affichage d'une partie à partir d'une seed
void loadTheGame(char* seed)
{
	printf("\nLoading Game ...\n");
	game g = getGameFromId(seed);
	//display_pieces(g -> pieces, g -> nb_pieces);
	printf("Done\n\n");
	while (!game_over_hr(g))
	{
		draw_interface(g, getIdFromGame(g));
		input_player(g);
	}
	draw_interface(g, getIdFromGame(g));
	printf("************\n*--- GG ---*\n************\n");
}

int main(int argc, char* argv[])
{
	// Construction de l'id : 0 -> Petit/Vertical | 1 -> petit/horizon | 2 -> grand/vertical | 3 -> grand/horizon
	char* Game1 = "4103300222052";
	char* Game2 = "8103022024034144301040250";
	char* Game3 = "8123024012040042145250301";

	loadTheGame(Game1);
	loadTheGame(Game2);
	loadTheGame(Game3);
	
	return 0;
}
