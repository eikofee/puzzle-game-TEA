#include <stdlib.h>
#include <stdio.h>
#include <game_rh.h>
#include <utility.h>
#include <interface_txt.h>

//Fonction qui permet le chargement et l'affichage d'une partie a partir d'un id
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

	//On recupère l'id de la game, et on affiche une première fois l'interface
	getIdFromGame(g, id);
	drawInterface(g, id);

	while (!game_over_hr(g))
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

int main(int argc, char* argv[])
{
	//On ecrit le nom du jeu en cours dans config.ini
	initFileConfig("rush-hour");
	if (argv[1])

{	char* id = (char*) malloc(sizeof(char) * 256);
		game g = getGameFromConfigFile(argv[1]);
		getIdFromGame(g, id);
		drawInterface(g, id);
	
		while (!game_over_hr(g))
	{
		printf("Enter the car's number you want to move :\n");
		inputPlayer(g, id);
		drawInterface(g, id);
	}
}

	char* Game0 = "13n6x6p2w2h1x2y3p2w3h1x0y5p1w1h2x3y4p1w1h3x4y3p1w1h3x5y3p1w1h2x0y3p2w2h1x1y4p2w2h1x0y2p1w1h2x2y1p2w2h1x4y1p1w1h2x1y0p2w2h1x2y0p2w2h1x4y0";
	//					 |0--------|13-------|4--------|5--------|6--------|1--------|14-------|16-------|3--------|17-------|2--------|18-------|24-------|
	char* Game1 = "4n6x6p2w2h1x0y3p2w3h1x0y0p1w1h3x2y2p1w1h2x5y2";
	char* Game2 = "8n6x6p2w2h1x0y3p1w1h2x2y2p1w1h2x2y4p1w1h2x3y4p2w2h1x4y4p2w3h1x0y1p1w1h2x4y0p1w1h3x5y0";
	char* Game3 = "8n6x6p2w2h1x2y3p1w1h2x2y4p1w1h2x1y2p1w1h2x4y0p1w1h2x4y2p2w2h1x4y5p1w1h3x5y0p2w3h1x0y1";
	
	loadTheGame(Game1);
	loadTheGame(Game0);
	loadTheGame(Game2);
	loadTheGame(Game3);
	return 0;
}