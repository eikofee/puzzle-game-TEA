#include <stdio.h>
#include <stdlib.h>
#include "game.c"

void display_pieces(piece *p, int taille)
{
	for (int i = 0; i < taille; i++)
	{
		printf("Pièce [%d] : x = %d, y = %d\n",i ,p[i] -> position[0], p[i] -> position[1]);
	}

}
int main(int argc, char* argv[])
{
	printf("> Préparation des tests:\n");
	printf("> création des pièces...\n");
	int nb_pieces = 4;
	piece *p = (piece*) malloc(sizeof( struct piece_s) * nb_pieces);
	p[0] = new_piece_rh(0, 3, true, true);		//Voiture rouge
	p[1] = new_piece_rh(2, 4, false, false);	//Camion jaune sur l'ex
	p[2] = new_piece_rh(5, 3, true, false);		//Voiture verte sur l'ex
	p[3] = new_piece_rh(0, 0, false, true);		//Camion horizontal en 0,0
	printf("Done.\n");

	printf("> new_game_hr...\n");
	game g = new_game_hr(nb_pieces, p);
	display_pieces(g -> pieces, nb_pieces);
	printf("Done.\n");

	printf("> création d'un jeu vide...\n");
	piece *q = (piece*) malloc(sizeof(struct piece_s));
	if (!q)
		error("Allocation de q");
	q[0] = new_piece_rh(0, 3, true, true);
	if (!q[0])
		error("Creation de q[0]");
	game g2 = new_game_hr(1, q);
	display_pieces(g2 -> pieces, g2 -> nb_pieces);
	printf("Done.\n");

	printf("> copy_game...\n");
	copy_game(g, g2);
	display_pieces(g2 -> pieces, g2 -> nb_pieces);
	printf("Done.\n");

	printf("> delete_game...\n");
	delete_game(g2);
	printf("Done.\n");

	printf("> game_nb_piece...\n");
	int a = game_nb_pieces(g);
	printf("Done, attendu = %d, reçu = %d.\n", nb_pieces, a);


	return 0;
}

