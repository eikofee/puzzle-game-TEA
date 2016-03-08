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
	bool result = true;
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
	for(int i = 0; i < nb_pieces; i++)
	{
		piece p_tmp = (piece)malloc(sizeof(struct piece_s));
		copy_piece(game_piece(g, i),p_tmp);
		printf("Valeur de x attendue : %d || Valeur obtenue : %d\n", get_x(p[i]), get_x(p_tmp));
		if(get_x(p[i]) != get_x(p_tmp))
			return;
		printf("Valeur de y attendue : %d || Valeur obtenue : %d\n", get_y(p[i]), get_y(p_tmp));
		if(get_y(p[i]) != get_y(p_tmp))
			return;

		printf("Valeur de isHorizontal attendue : %d || Valeur obtenue : %d\n", is_horizontal(p[i]), is_horizontal(p_tmp));
		if(is_horizontal(p[i]) != is_horizontal(p_tmp))
			return;

		printf("Valeur de longueur x largeur attendu : %d x %d || Valeur obtenue : %d x %d \n\n", get_width(p[i]), get_height(p[i]), get_width(p_tmp), get_height(p_tmp));
		if(get_width(p[i]) != get_width(p_tmp) || get_height(p[i]) != get_height(p_tmp))
			return;

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
	display_pieces(g2 -> pieces, game_nb_pieces(g2));
	printf("Done.\n\n");

	printf("> copy_game...\n");
	copy_game(g, g2);
	
	printf("Pieces src : \n");
	display_pieces(g -> pieces, game_nb_pieces(g2));
	if (game_nb_pieces(g) != game_nb_pieces(g2))
		return;
	for (int i; i < game_nb_pieces(g2); i++)
	{
		if(get_x(game_piece(g,i)) != get_x(game_piece(g2,i)))
			return;
		if(get_y(game_piece(g,i)) != get_y(game_piece(g2,i)))
			return;
		if(is_horizontal(game_piece(g,i)) != is_horizontal(game_piece(g2,i)))
			return;
		if(get_width(game_piece(g, i)) != get_width(game_piece(g2, i)) || get_height(game_piece(g, i)) != get_height(game_piece(g2, i)))
			return;
	}
	printf("Pièces dst : \n");
	display_pieces(g -> pieces, game_nb_pieces(g2));
	printf("Done.\n\n");

	printf("> delete_game(g2)...\n");
	delete_game(g2);
	printf("Done.\n\n");

	printf("> game_nb_piece...\n");
	int a = game_nb_pieces(g);
	printf("Done, attendu = %d, reçu = %d.\n\n", nb_pieces, a);
	if (nb_pieces != a)
		return;
	printf("play_move\n");
	if (play_move(g,0,LEFT,-1))
		return;	
	if (!play_move(g,1,UP,1))
		return;
	if (!play_move(g,2,DOWN,-1))
		return;
	if (!play_move(g,3,RIGHT,1))
		return;
	printf("> delete_game(g)...\n");
	delete_game(g);
	printf("Done.\n\n");

	delete_piece(p[0]);
	delete_piece(p[1]);
	delete_piece(p[2]);
	delete_piece(p[3]);
	delete_piece(q[0]);

	if(result)
		printf("Good.");
}

