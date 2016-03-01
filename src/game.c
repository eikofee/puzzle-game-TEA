#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "utility.h"


game new_game_hr( int nb_pieces, piece *pieces)
{
	game new_game = (game) malloc(sizeof(struct game_s));

	if(!new_game)
		error("Allocation new_game");

	new_game -> pieces = (piece*) malloc(sizeof(struct piece_s) * nb_pieces);
	
	if(!new_game -> pieces)
		error("Allocation new_game -> pieces");

	//Affectation des valeurs par copie
	new_game -> nb_pieces = nb_pieces;
	for (int i = 0; i < nb_pieces; i++)
	{
		new_game -> pieces[i] = new_piece_rh(0,0,true,true);

		if (!new_game -> pieces[i])
			error("Allocation new_game -> pieces[i]");

		copy_piece(pieces[i], new_game -> pieces[i]);

		if (!new_game -> pieces[i])
		{
			printf("i=%d\n", i);
			error("Copie de pièce");
		}
	}

	//Initialisation des valeurs de jeu
	new_game -> nb_moves = 0;
	return new_game;
}

void delete_game (game g)
{
	if(g != NULL)
	{
		//On libère d'abord le tableau des pieces
		for (int i = 0; i < g -> nb_pieces; i++)
		{
			delete_piece(g -> pieces[i]);
		}
		free(g -> pieces);
		//...Ensuite le reste de la structure
		free(g);
	}
}

void copy_game(cgame src, game dst)
{
	if(src == NULL || dst == NULL)
		error("copy_game, src ou dst ne sont pas alloué");

	//On supprime les anciennes pieces
	for (int i = 0; i < dst -> nb_pieces; i++)
		delete_piece(dst -> pieces[i]);
	free(dst -> pieces);

	//D'abord les propriétés directes
	dst -> nb_pieces = src -> nb_pieces;
	printf("nb_pieces Done : attendu : %d, recu : %d.\n", src->nb_pieces, dst->nb_pieces);

	dst -> nb_moves = src -> nb_moves;
	printf("nb_moves Done : attendu : %d, recu : %d.\n", src->nb_moves, dst->nb_moves);

	//...Ensuite le tableau des pièces
	//On réalloue du tableau des pieces
	dst -> pieces = (piece*) malloc(sizeof(struct piece_s) * dst -> nb_pieces);

	if (!dst -> pieces)
		error("Echec du deuxième malloc");

	for (int i = 0; i < src -> nb_pieces; i++)
	{
		dst -> pieces[i] = (piece) malloc(sizeof(struct piece_s));

		if (!dst -> pieces[i])
			error("Allocation dst -> pieces[i]");

		copy_piece(src -> pieces[i], dst -> pieces[i]);
	}
}

int game_nb_pieces(cgame g)	//Not sure about this one tbh
{
	if (!g)
		error("Allocation cgame game_nb_pieces");

	return g -> nb_pieces;
}

cpiece game_piece(cgame g, int piece_num)
{
	//Vérifie que l'indice piece_num pièce est bien dans le jeu
    if (piece_num < 0 || piece_num > game_nb_pieces(g) - 1)
		error("L'index de la pièce recherchée est impossible (trop grand ou négatif)");

	//Si elle existe, on la renvoie
	return g -> pieces[piece_num];
}

bool game_over_hr(cgame g)
{
	//On regarde si les coordonnées de la voiture 0 sont bien (4;3)
	if (get_x(g -> pieces[0]) == 4 && get_y(g -> pieces[0]) == 3)
		return true;

	return false;
}

bool play_move(game g, int piece_num, dir d, int distance)
{
    //On vérifie qu'il n'entre au contact d'aucune pièce et qu'il reste sur le plateau en utilisant un clone-cobaye (ptest)
	piece ptest = new_piece_rh(0, 0, true, true);
	copy_piece(g -> pieces[piece_num], ptest);

	int ptestx = get_x(ptest);
	int ptesty = get_y(ptest);

	//Cette première boucle permet de faire pas à pas le déplacement lorsque celui ci est supérieur à 1
	for (int step = 0; step < abs(distance); step++)
	{
		move_piece(ptest,d,1);
		if(!estPositionValide(ptest))
		{
			delete_piece(ptest);
			return false;
		}

		//Boucle qui vérifie la colision avec les autres pieces
		for (int i = 0; i < game_nb_pieces(g); i++)
		{
        	// s'il ne s'agit pas de la même pièce, alors on regarde s'il y a contact.
			if (i != piece_num && intersect(ptest, g -> pieces[i]))
			{
				delete_piece(ptest);
				return false;
			}
		}	
	}
	// Si ptest n'a pas bougé, c'est que move_piece a trouvé que le mouvement sur distance n'était pas bon.
	if (distance != 0 && ((!is_horizontal(ptest) && get_y(ptest) != ptesty + distance) || (is_horizontal(ptest) && get_x(ptest) != ptestx + distance))) {
		delete_piece(ptest);
		return false;
	}

	delete_piece(ptest);
    //Si tout est bon, alors on lance l'algorithme de déplacement de la pièce, puis on ajoute les moves correspondant.
	move_piece(g -> pieces[piece_num], d, abs(distance));
	g -> nb_moves += abs(distance);
	return true;
}

int game_nb_moves(cgame g)
{
	if(g != NULL)
    	return g -> nb_moves;
    
    return -1;
}

//error est une fonction qui permet d'envoyer un message sur stderr et de faire un exit(EXIT_FAILURE). 
//Elle permet d'éviter de recopier a chaque fois 2 à 3 lignes.
void error(char* s)
{
	fprintf(stderr, "Fatal Error : %s.\n", s);
	exit(EXIT_FAILURE);
}
