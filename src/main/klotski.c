#include <stdlib.h>
#include <stdio.h>
#include <game_rh.h>
#include <game_ar.h>
#include <utility.h>
#include <interface_txt.h>

// Cette fonction sert a charger une partie et d'y jouer.
void loadTheGame(char* id_src)
{
	//On crée le game g à partir d'un id source
	printf("\nLoading Game ...\n");
	game g = getGameFromId(id_src);
	printf("Done\n\n");

	//On malloc un id qu'on utilisera pour etre modifié pendant la partie
	char* id = (char*) malloc(sizeof(char) * 256);
	if(id == NULL)
		error("getIdFromGame(), probleme d'allocation memoire");

	getIdFromGame(g, id);
	drawInterface(g, id);

	while (!game_over_ar(g))
	{
		printf("Enter the car's number you want to move :\n");
		inputPlayer(g, id);
		drawInterface(g, id);
	}

	getIdFromGame(g, id);

	printf("************\n*--- GG ---*\n************\n");

	free(id);
	delete_game(g);
}

int main(void)
{
	//On ecrit le nom de l'exucutable dans le fichier config.ini puis on lance le jeu
	initFileConfig("klotski");

	char* Game1 = "10n4x5p3w2h2x2y0p3w1h2x1y0p3w1h2x0y0p3w2h1x0y2p3w2h1x2y2p3w1h2x0y3p3w1h1x1y3p3w1h1x1y4p3w1h1x2y3p3w1h1x2y4";
	char* Game2 = "10n4x5p3w2h2x2y0p3w1h1x1y0p3w1h1x0y0p3w1h1x0y1p3w1h1x1y1p3w2h1x0y2p3w2h1x2y2p3w2h1x0y3p3w2h1x0y4p3w2h1x2y4";
	char* Game3 = "15n4x5p3w2h2x2y1p3w1h1x0y0p3w1h1x1y0p3w1h1x2y0p3w1h1x3y0p3w1h1x0y1p3w1h1x1y1p3w1h1x0y2p3w1h1x1y2p3w1h1x0y3p3w1h1x0y4p3w1h1x2y3p3w1h1x3y3p3w1h1x2y4p3w1h1x3y4";
	loadTheGame(Game1);
	loadTheGame(Game2);
	loadTheGame(Game3);

	return 0;
}

