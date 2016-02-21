#include <stdio.h>
#include <stdlib.h>
#include "game.c"
#include "utility.h"

void draw_interface();
void input_player();

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
	p[1] = new_piece_rh(2, 2, true, false);	//Camion jaune sur l'ex
	p[2] = new_piece_rh(5, 2, true, false);		//Voiture verte sur l'ex
	p[3] = new_piece_rh(0, 0, false, true);		//Camion horizontal en 0,0
	game g = new_game_hr(nb_pieces, p);

	return g;
}

int main(int argc, char* argv[])
{
	game g = test_level();
	//display_pieces(g -> pieces, g -> nb_pieces);
	while (!game_over_hr(g))
	{
		draw_interface(g);
		input_player(g);
	}
	return 0;
}

void draw_interface(game g)
{
	//Test values
	int moves = game_nb_moves(g);
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

bool str_equal(char* a, char* b)
{
	int i = 0;
	while (a[i] != '\n')
	{
		if (a[i] != b[i])
			return false;
		i++;
	}
	return true;
}

bool isNumber(char s, int max_number)
{
	return (s >= 48 && s <= 48 + max_number);
}
bool isOperatorSimple(char s)
{
	return (s == '+' || s == '-');
}
int getNumber(char s)
{
	return s - 48;
}
dir getDirection(piece p, char sign)
{
	if (sign == '+')
	{
		if (p -> isHorizontal)
			return RIGHT;
		else{
			return UP;
		}
	}else{
		if (p -> isHorizontal)
			return LEFT;
		else{
			return DOWN;
		}
	}
}

void input_player(game g)
{
	char input[7] = "";
	fgets(input, 6, stdin);

	if (str_equal(input, "help"))
	{
		//Display help
	}
	if (str_equal(input, "hint"))
	{
		//Do a move 
	}
	if (str_equal(input, "exit"))
	{
		exit;
	}
	if (str_equal(input, "save"))
	{
		//sauvegarde
	}
	if (isNumber(input[0], g -> nb_pieces))
	{
		//correct input
		/*SYNTAXE :
			0 2 : Avance la voiture rouge de 2 cases vers la droite
			1 -1 : Recule la voiture 1 vers le bas si verticale ou la gauche si horizontale
			2 : Demande un déplacement de la voiture 2 (nouveau scanf)
		*/ 
			//syntaxe check
			if (input[1] == '\n')
			{
				//ask for second input
			}else{
				if (input[1] == ' ' && isNumber(input[2], 9))
				{
					//TODO : Check if piece actually exists
					play_move(g, getNumber(input[0]), getDirection(g -> pieces[getNumber(input[0])], '+'), getNumber(input[2]));
				}else{
					if (isOperatorSimple(input[2]))
					{
						if (input[2] == '+')
							play_move(g, getNumber(input[0]), getDirection(g -> pieces[getNumber(input[0])], '+'), getNumber(input[3]));
						else{
							play_move(g, getNumber(input[0]), getDirection(g -> pieces[getNumber(input[0])], '-'), getNumber(input[3]));
						}
					}
				}
			}
		}else{
			printf("Incorrect input.\n");
		}

	}