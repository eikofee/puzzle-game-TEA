#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "utility.h"

void display_pieces(piece *p, int taille)
{
	for (int i = 0; i < taille; i++)
	{
		printf("Pièce [%d] : x = %d, y = %d\n",i ,get_x(p[i]), get_y(p[i]));
	}
}


int main(int argc, char* argv[])
{
	printf("> Préparation des tests:\n");
	printf("> création des pièces...\n");
	int nb_pieces = 4;
	piece p[nb_pieces];
	p[0] = new_piece_rh(0, 3, true, true);		//Voiture rouge
	p[1] = new_piece_rh(2, 2, false, false);	//Camion jaune sur l'ex
	p[2] = new_piece_rh(5, 2, true, false);		//Voiture verte sur l'ex
	p[3] = new_piece_rh(0, 0, false, true);		//Camion horizontal en 0,0
	printf("Done.\n\n");

	printf("> new_game_hr...\n");
	game g = new_game_hr(nb_pieces, p);
	display_pieces(g -> pieces, nb_pieces);
	printf("Done.\n\n");

	printf("> game_piece...\n");
	bool result = true;
	for(int i = 0; i < nb_pieces; i++)
	{
		piece p_tmp = game_piece(g, i);
		printf("Valeur de x attendue : %d || Valeur obtenue : %d\n", get_x(p[i]), get_x(p_tmp));
		if(get_x(p[i]) != get_x(p_tmp))
			result = false;
		printf("Valeur de y attendue : %d || Valeur obtenue : %d\n", get_y(p[i]), get_y(p_tmp));
		if(get_y(p[i]) != get_y(p_tmp))
			result = false;

		printf("Valeur de isHorizontal attendue : %d || Valeur obtenue : %d\n", is_horizontal(p[i]), is_horizontal(p_tmp));
		if(is_horizontal(p[i]) != is_horizontal(p_tmp))
			result = false;

		printf("Valeur de isSmall attendu : %d || Valeur obtenue : %d\n\n", p[i]->isSmall, p_tmp->isSmall);
		if(p[i] -> isSmall != p_tmp -> isSmall)
			result = false;

		delete_piece(p_tmp);
	}
	printf(result ? "game_piece Done\n\n" : "Erreur: game_piece\n\n");

	printf("> création d'un jeu vide...\n");
	piece q[nb_pieces];

	if (!q)
		fprintf(stderr, "Allocation de q");

	q[0] = new_piece_rh(0, 3, true, true);

	if (!q[0])
		fprintf(stderr, "Creation de q[0]");

	game g2 = new_game_hr(1, q);
	display_pieces(g2 -> pieces, g2 -> nb_pieces);
	printf("Done.\n\n");

	printf("> copy_game...\n");
	copy_game(g, g2);
	
	printf("Pieces src : \n");
	display_pieces(g -> pieces, g -> nb_pieces);

	printf("Pièces dst : \n");
	display_pieces(g2 -> pieces, g2 -> nb_pieces);
	printf("Done.\n\n");

	printf("> delete_game(g2)...\n");
	delete_game(g2);
	printf("Done.\n\n");

	printf("> game_nb_piece...\n");
	int a = game_nb_pieces(g);
	printf("Done, attendu = %d, reçu = %d.\n\n", nb_pieces, a);

	printf("> delete_game(g)...\n");
	delete_game(g);
	printf("Done.\n\n");

	delete_piece(p[0]);
	delete_piece(p[1]);
	delete_piece(p[2]);
	delete_piece(p[3]);
	delete_piece(q[0]);

	return 0;
}

