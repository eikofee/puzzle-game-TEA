#include <stdio.h>
#include <stdlib.h>
#include <game.h>
#include <game_ar.h>
#include <utility.h>

game new_game_ar( int nb_pieces, piece *pieces)
{
	game newGame_ar = new_game(4, 5, nb_pieces, pieces);
	if(newGame_ar == NULL)
		error("new_game_hr(), porbleme d allocation memoire");
	return newGame_ar;
}


bool game_over_ar(cgame g)
{
	return (get_x(game_piece(g, 0)) == 1 && get_y(game_piece(g, 0)) == 0);
}


/*Syntaxe version 2:
		(nb_pieces)n(taille_x)x(taille_y)p(1,2 ou 3)w(width)h(height)x(pos_x)y(pos_y)p(next)
	*/
game getGameFromIdAR(char* id)
{
	int i = 0;
	int nb_pieces = readUntilChar(id, &i);
	i++;
	piece p[nb_pieces];
	int n_piece = 0;	//index de p
	int taille_x = 0;
	int taille_y = 0;
	int state = 1; // 1 = taille_x, 2 = taille_y, 3 = piece
	piece p_tmp;
	

	while (id[i] != '\0')
	{
		switch(state)
		{
			case 1:
				taille_x = readUntilChar(id, &i);
				break;
			case 2:
				taille_y = readUntilChar(id, &i);
				break;
			case 3:

				p_tmp = getPieceFromIdAR(id, &i);
				p[n_piece] = new_piece(0, 0, 0, 0, true, true);
				copy_piece(p_tmp, p[n_piece]);
				n_piece++;
				state--;
				delete_piece(p_tmp);
				break;
		}
		if (id[i])
			i++;
		state++;
	}
	game g = new_game(taille_x, taille_y, nb_pieces, p);
	return g;
}