#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "piece.h"



struct game_s{
	piece *pieces;	//malloc nécessaire
	int	nb_moves;
};

game new_game_hr( int nb_pieces, piece *pieces)
{
	game new_game = (game_s) malloc(sizeof(struct game_s));
	new_game -> pieces = (piece) malloc(sizeof(struct piece_s) * nb_pieces);

	for (int i = 0; i < nb_pieces; i++)
	{
		new_game -> pieces[i] = pieces[i];
	}

	new_game -> nb_moves = 0;
	return new_game;
}

cpiece game_piece(cgame g, int piece_num){
  //Vérifie que la pièce est bien dans le jeu
  if (piece_num < 0 || piece_num > game_nb_pieces(g)){
    fprintf(stderr,"L'index de la pièce recherchée est impossible (trop grand ou négatif\n");
    exit(EXIT_FAILURE);
  }
  //Si elle existe, on la renvoie
  return g -> pieces[piece_num];
}


bool game_over_hr(cgame g){
  if (g -> pieces[0].position[0] = 4 && g -> pieces[0].position[1] = 3){
    return true;
  }
  return false;
}

