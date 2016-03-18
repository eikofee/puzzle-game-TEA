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
	char* id = (char*) malloc(sizeof(char) * 128);
	if(id == NULL)
		error("getIdFromGame(), probleme d'allocation memoire");

	//On recupère l'id de la game, et on affiche une première fois l'interface
	getIdFromGame(g, id);
	draw_interface(g, id);

	while (!game_over_hr(g))
	{
		printf("Enter the car's number you want to move :\n");
		input_player(g, id);
		draw_interface(g, id);
	}

	getIdFromGame(g, id);

	printf("************\n*--- GG ---*\n************\n");

	free(id);
	delete_game(g);
}

char* convertOldID(char* oldID){
	int i_oldID = 0;
	int i_newID = 0;
	char* newID = (char*)malloc(sizeof(char) * 256);
	newID[i_newID] = oldID[i_oldID];
	i_newID++;
	i_oldID++;

	newID[i_newID] = 'n';
	i_newID++;

	newID[i_newID] = '6';
	i_newID++;

	newID[i_newID] = 'x';
	i_newID++;

	newID[i_newID] = '6';
	i_newID++;

	while(oldID[i_oldID] != '\0')
	{
		newID[i_newID] = 'p';
		i_newID++;

		switch(oldID[i_oldID])
		{
			case '0':
				newID[i_newID] = '1';
				i_newID++;
				newID[i_newID] = 'w';
				i_newID++;
				newID[i_newID] = '1';
				i_newID++;
				newID[i_newID] = 'h';
				i_newID++;
				newID[i_newID] = '2';
				i_newID++;
				break;

			case '1':
				newID[i_newID] = '2';
				i_newID++;
				newID[i_newID] = 'w';
				i_newID++;
				newID[i_newID] = '2';
				i_newID++;
				newID[i_newID] = 'h';
				i_newID++;
				newID[i_newID] = '1';
				i_newID++;
				break;

			case '2':
				newID[i_newID] = '1';
				i_newID++;
				newID[i_newID] = 'w';
				i_newID++;
				newID[i_newID] = '1';
				i_newID++;
				newID[i_newID] = 'h';
				i_newID++;
				newID[i_newID] = '3';
				i_newID++;
				break;

			case '3':
				newID[i_newID] = '2';
				i_newID++;
				newID[i_newID] = 'w';
				i_newID++;
				newID[i_newID] = '3';
				i_newID++;
				newID[i_newID] = 'h';
				i_newID++;
				newID[i_newID] = '1';
				i_newID++;
				break;

			default:
				printf("ConvertoldID cas non prevu switch\n");
				break;
		}

		i_oldID++;
		newID[i_newID] = 'x';
		i_newID++;
		newID[i_newID] = oldID[i_oldID];
		i_newID++;
		i_oldID++;
		newID[i_newID] = 'y';
		i_newID++;
		newID[i_newID] = oldID[i_oldID];
		i_oldID++;
		i_newID++;

	}
	newID[i_newID] = '\0';
	return newID;
}

int main(int argc, char* argv[])
{
	initFileConfig("rush-hour");
	// 0 -> isSmall | Vertical
	// 1 -> isSmall | Horizontal
	// 2 -> Grand | Vertical
	// 3 -> Grand | Horizontal

	// char* Game1_tmp = "4103300222052";
	// char* Game2 = "8103022024034144301040250";
	// char* Game3 = "8123024012040042145250301";
	
	char* Game1 = "4n6x6p2w2h1x0y3p2w3h1x0y0p1w1h3x2y2p1w1h2x5y2";
	char* Game2 = "8n6x6p2w2h1x0y3p1w1h2x2y2p1w1h2x2y4p1w1h2x3y4p2w2h1x4y4p2w3h1x0y1p1w1h2x4y0p1w1h3x5y0";
	char* Game3 = "8n6x6p2w2h1x2y3p1w1h2x2y4p1w1h2x1y2p1w1h2x4y0p1w1h2x4y2p2w2h1x4y5p1w1h3x5y0p2w3h1x0y1";
	
	loadTheGame(Game1);
	loadTheGame(Game2);
	loadTheGame(Game3);
	return 0;
}