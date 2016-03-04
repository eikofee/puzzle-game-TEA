#include <stdlib.h>
#include <stdio.h>
#include "game.h"
#include "utility.h"
#include "interface_txt.h"

//Fonction qui permet le chargement et l'affichage d'une partie �? partir d'une seed
void loadTheGame(char* id_src)
{
	printf("\nLoading Game ...\n");
	game g = getGameFromId(id_src);
	printf("Done\n\n");

	char* id = (char*) malloc(sizeof(char) * 128);
	if(id == NULL)
		error("getIdFromGame(), problème d'allocation mémoire");

	getIdFromGame(g, id);
	draw_interface(g, id);

	while (!game_over_hr(g))
	{
		input_player(g, id);
		game g2 = getGameFromId(id);
		copy_game(g2, g);
		delete_game(g2);
		draw_interface(g, id);
		
	}

	getIdFromGame(g, id);
	draw_interface(g, id);

	printf("************\n*--- GG ---*\n************\n");

	free(id);
	delete_game(g);
}

int main(int argc, char* argv[])
{
	char* Game1 = "4103300222052";
	char* Game2 = "8103022024034144301040250";
	char* Game3 = "8123024012040042145250301";
	//char* Game0 = "f103105104102101100125124123122121120145144143";

	// loadTheGame(Game0);
	loadTheGame(Game1);
	// loadTheGame(Game2);
	// loadTheGame(Game3);
	
	return 0;
}