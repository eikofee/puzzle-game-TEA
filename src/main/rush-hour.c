#include <stdlib.h>
#include <stdio.h>
#include <game.h>
#include <utility.h>
#include <interface_txt.h>

//Fonction qui permet le chargement et l'affichage d'une partie a partir d'un id
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

	while (!game_over_hr(g))
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

int main(int argc, char* argv[])
{
	// char* Game1 = "4103300222052";
	// char* Game2 = "8103022024034144301040250";
	// char* Game3 = "8123024012040042145250301";
	
	//loadTheGame(Game1);
	//loadTheGame(Game2);
	//loadTheGame(Game3);
	game g = getGameFromIdAR("3n10x10p3w4h2x2y2p3w2h2x6y6p3w10h2x0y8");
	char* s = (char*) malloc(sizeof(char) * 256);
	while (true)
	{
		getIdFromGameAR(g, s);
		draw_interface(g, s);
		input_player(g, s);
	}

	return 0;
}