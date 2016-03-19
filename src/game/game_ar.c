#include <stdio.h>
#include <stdlib.h>
#include <game.h>
#include <game_ar.h>
#include <utility.h>

game new_game_ar( int nb_pieces, piece *pieces)
{
	//Les valeurs 4 et 5 correspondent à la largeur et à la hauteur du jeu de l'Ane Rouge
	game newGame_ar = new_game(4, 5, nb_pieces, pieces);
	if(newGame_ar == NULL)
		error("new_game_hr(), porbleme d allocation memoire");
	return newGame_ar;
}

bool game_over_ar(cgame g)
{
	//Ici on regarde juste si la pièce d'indice 0 est bien a les coordonnées (1;0)
	return (get_x(game_piece(g, 0)) == 1 && get_y(game_piece(g, 0)) == 0);
}