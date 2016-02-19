#include <stdio.h>
#include <stdlib.h>
#include "game.c"
#include "utility.h"

void draw_interface();

void display_pieces(piece *p, int taille)
{
	for (int i = 0; i < taille; i++)
	{
		printf("Pièce [%d] : x = %d, y = %d\n",i ,get_x(p[i]), get_y(p[i]));
	}

}
game test_level()
{

	int nb_pieces = 4;
	piece p[nb_pieces];
	p[0] = new_piece_rh(0, 3, true, true);		//Voiture rouge
	p[1] = new_piece_rh(2, 2, false, false);	//Camion jaune sur l'ex
	p[2] = new_piece_rh(5, 2, true, false);		//Voiture verte sur l'ex
	p[3] = new_piece_rh(0, 0, false, true);		//Camion horizontal en 0,0
	game g = new_game_hr(nb_pieces, p);

	return g;
}

int main(int argc, char* argv[])
{
	game g = test_level();
	//display_pieces(g -> pieces, g -> nb_pieces);
	draw_interface(g);
	return 0;
}

void draw_interface(game g)
{
	//Test values
	int moves = 3;
	int num_moves = 10;
	char* seed = "0b4aaf345e";
	int** t = TableauDePieces(g -> pieces, g -> nb_pieces);
	printf("############### Rush Hour\n");

	for (int i = 5; i > -1; i--)
	{
		printf("# ");
		for (int j = 0; j < 6; j++)
		{
			if (t[j][i] == -1)
				printf(".");
			else
			{
				printf("%d", t[j][i]);
			}
			printf(" ");
		}
		
		switch(i){
			//let's check if we have to display more informations on the right:
			case 4:	//Move num display
				printf("# Move %d / %d\n", moves, num_moves);
				break;
			case 1:	//Level's seed display (optional)
				printf("# Seed: %s\n", seed);
				break;
			case 3: //Exit
				printf(">\n");
				break;
			case 0: 
				printf("# Type 'help' for more informations\n");
				break;
			default:
				printf("#\n");
				break;
		}
	}
	printf("###############\n");
	printf("Enter the car's number you want to move :\n");
}
