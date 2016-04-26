#include <stdlib.h>
#include <stdio.h>
#include <game_rh.h>
#include <game_ar.h>
#include <utility.h>
#include <interface_txt.h>
#include <string.h>

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

int main(int argc, char* argv[])
{
	//On ecrit le nom de l'exucutable dans le fichier config.ini puis on lance le jeu
	initFileConfig("klotski");
		//Permet de savoir quelle ligne lire.
	int indGame = 1;

	while(indGame < 4)//Nombre de niveau + 1 dans le fichier .txt contenant les niveaux
	{
		char* idGame = loadGameFromFile("games_ar.txt", indGame);
		loadTheGame(idGame);
		indGame++;
		free(idGame);
	}

	return 0;
}

