#include <stdio.h>
#include <stdlib.h>
#include "game.c"


int main(int argc, char* argv[])
{
	printf("> Préparation des tests:\n");
	printf("> création des pièces...");
	int nb_pieces = 4;
	piece *p = (piece*) malloc(sizeof( struct piece_s) * nb_pieces);
	p[0] = new_piece_rh(0, 3, true, true);		//Voiture rouge
	p[1] = new_piece_rh(2, 4, false, false);	//Camion jaune sur l'ex
	p[2] = new_piece_rh(5, 3, true, false);		//Voiture verte sur l'ex
	p[3] = new_piece_rh(0, 0, false, true);		//Camion horizontal en 0,0
	printf("Done.\n");

	printf("> new_game_hr...");
	game g = new_game_hr(nb_pieces, p);
	printf("Done.\n");

	printf("> copy_game...");
	game g2;
	copy_game(g, g2);
	printf("Done.\n");

	printf("> delete_game...");
	delete_game(g2);
	printf("Done.\n");

	printf("> game_nb_piece...");
	int a = game_nb_piece(g);
	printf("Done, attendu = %d, reçu = %d.\n", nb_pieces, a);


	return 0;
}

