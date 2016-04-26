#include <stdlib.h>
#include <stdio.h>
#include <interface_sdl.h>


int main(){
	//On affiche le menu du choix du jeu.
	int check = choixDuJeu();
	if(check == -1)
		return EXIT_SUCCESS;

	//ces deux variables permettent de charger les niveaux dans les fichiers txt.
	int indGame = 1;
	char *strIndGame = (char*)malloc(5 * sizeof(char));

	check = 1;
	while(check && indGame < 4)//la deuxieme condition permet juste d'éviter de dépasser la fin du fichier txt.
	{
		char *Game1 = (char*)malloc(512 * sizeof(char));
		sprintf(strIndGame, "%d\n", indGame);
		if(whatGame("rush-hour\n"))
			Game1 = loadGameFromNum("games_rh.txt", strIndGame);

		else if(whatGame("klotski\n"))
			Game1 = loadGameFromNum("games_ar.txt", strIndGame);
		
		game g = getGameFromId(Game1);
		init_sdl_game(g, &check, indGame);
		delete_game(g);
		indGame++;
		free(Game1);
	}
	free(strIndGame);
	return EXIT_SUCCESS;
}