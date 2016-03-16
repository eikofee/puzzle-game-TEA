#include <stdio.h>
#include <stdlib.h>
#include <game.h>
#include <game_rh.h>
#include <utility.h>

#define TAILLE_PLATEAU_RH 6

game new_game_hr( int nb_pieces, piece *pieces)
{
	game newGame_rh = new_game(TAILLE_PLATEAU_RH, TAILLE_PLATEAU_RH, nb_pieces, pieces);
	if(newGame_rh == NULL)
		error("new_game_hr(), porbleme d allocation memoire");
	return newGame_rh;
}


bool game_over_hr(cgame g)
{
	int x_voiture = get_x(game_piece(g, 0));
	return (x_voiture == (game_width(g) - get_width(game_piece(g, 0))));
}

/*
	Permet de générer un niveau à partir d'un id (non seed)
*/
game getGameFromIdRH(char* id)
{
	//Syntaxe : Nabca2b2c2a3b3c3
	//N = nombre de voitures
	//a = car type: +1 st horizontale, +2 si grand
	//b = case axe x
	//c = case axe y

	int nb_pieces = getNumber(id[0]) ;
	piece p[nb_pieces];
	int i = 1;
	int indexP = 0;

	while (id[i] != '\0')
	{
		bool isHorizontal = (id[i] == '1' || id[i] == '3');
		bool isSmall = (id[i] == '0' || id[i] == '1');
		int x = getNumber(id[i + 1]);
		int y = getNumber(id[i + 2]);
		p[indexP] = new_piece_rh(x, y, isSmall, isHorizontal); //isSmall
		i += 3;
		indexP++;
	}
	game g = new_game_hr(nb_pieces, p);

	for(int j = 0; j < indexP; j++)
		free(p[j]);

	return g;
}