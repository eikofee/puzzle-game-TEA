#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"



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
    if (piece_num < 0 || piece_num > game_nb_pieces(g)-1){
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

bool play_move(game g, int piece_num, dir d, int distance){
    //On vérifie que le mouvement est correct par rapport à la pièce (Si incorrect, return false)
    if (((d == "UP" || d == "DOWN") &&  g -> pieces[piece_num].isHorizontal) || (d == "LEFT" || d == "RIGHT") && g -> pieces[piece_num].isHorizontal == false){
        return false;
    }
    //On vérifie que si le mouvement est réalisé il reste sur la plateforme (Sinon, false)
    if ((d == "UP" && (g -> pieces[piece_num].position[1] + distance > 5)) || (d == "DOWN" && (g -> pieces[piece_num].position[1] + distance < 0)) || (d == "RIGHT" && (g -> pieces[piece_num].position[0] + distance > 5)) || (d == "LEFT" && (g -> pieces[piece_num].position[0] + distance > 0))){
        return false;
    }
    //On vérifie qu'il n'entre au contact d'aucune pièce
    for (int i = 0; i < game_nb_pieces(g); i++){
        if ((d == "UP" || d == "DOWN") && (g -> pieces[piece_num].position[1] + distance) == g -> pieces[i].position[1] || (d == "LEFT" || d == "RIGHT") && (g -> pieces[piece_num].position[0] + distance) == g -> pieces[i].position[0]) {
            return false;
        }
    }
    //Si tout est bon, alors on déplace la pièce, puis on ajoute les moves, puis le signal est bon
    switchsMovePiece(g -> pieces[piece_num], d, distance);
    g -> nb_moves += abs(distance);
    return true;
}

int game_nb_moves(cgame g){
    return g -> nb_moves;
}

