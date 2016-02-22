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
	p[1] = new_piece_rh(2, 2, false, false);	    //Camion jaune sur l'ex
	p[2] = new_piece_rh(5, 2, true, false);		//Voiture verte sur l'ex
	p[3] = new_piece_rh(0, 0, false, true);		//Camion horizontal en 0,0
	game g = new_game_hr(nb_pieces, p);

	return g;
}

long revertLong(long n)
{
	long a = 7;
	while (n != 7)
	{
		a *=10;
		a = n % 10;
		n /= 10;
	}
	a = a * 10 + 7;
	return a;
}
int getNbPieces(long l)
{
	int n = 0;
	while (l != 0)
	{
		l /= 10;
		n++;
	}
	n -= 2;
	return n / 3 + 1;
}
//73002221527
//---------
void printl(long i)
{
	printf("id=%ld\n", i);
}
game getGameFromId(long id)
{
	//Syntaxe : 7abca2b2c2a3b3c37
	//a = car type: +1 st horizontale, +2 si grand
	//b = case axe x
	//c = case axe y
	//7 to get start
	printl(id);
	id = revertLong(id);
	printl(id);
	int nb_pieces = getNbPieces(id);
	id /= 10;
	piece p[nb_pieces];
	p[0] = new_piece_rh(0, 3, true, true);
	int index = 1;
	while (id != 7)
	{
		int c = id%10;
		bool isHorizontal = (c == 1 || c == 3);
		bool isSmall = (c == 0 || c == 1);
		id /= 10;
		int x = id%10;
		id /= 10;
		int y = id%10;
		id /= 10; //Prochaine pièce
		printf("x=%d; y=%d;\n", x, y);
		p[index] = new_piece_rh(x, y, isSmall, isHorizontal);
		index++;
	}
	return new_game_hr(nb_pieces, p);
}

int main(int argc, char* argv[])
{
	//game g = test_level();
	//display_pieces(g -> pieces, g -> nb_pieces);
	game g = getGameFromId(73002221527);
	while (!game_over_hr(g))
	{
		draw_interface(g);
		input_player(g);
	}
	draw_interface(g);
	printf("************\n*--- GG ---*\n************\n");
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
void confirm()
{
	printf("\n\tPress 'Enter' to go back to the help menu");
	int cha;
	while (cha != '\r' && cha != '\n')
		cha = getchar();	
}
void getHelp(int input, bool* done)
{
	switch(input)
	{
		case 4:
		*(done) = true;
		break;
		case 1:
		printf("\tRules are simple. You're the car n°0 and you need to go to the exit (\">\"). To do that, you need\n");
		printf("to move the other cars to free yourself a passage. Cars can't cross others or go outside the game area.\n");
		confirm();
		break;
		case 2:
		printf("\tSyntaxe is as it follows : a b where a is the car number you want to move and b the distance to\n");
		printf("translate it. Positive numbers go to up and right and negative ones go to down and left.\n");
		printf("e.g. 2 -3 with the car number 2 being a horizontal car will go 3 cases to the left. You can also input\n");
		printf("the car number first and then input the distance.\n");
		confirm();
		break;
		case 3:
		printf("\tList of available commands:\n\n");
		printf("\thelp: Display this menu\n");
		printf("\thint: Get closer to the end by cheating\n");
		printf("\tskip: skip the current game for another\n");
		printf("\tsave: Save the current run for later\n");
		printf("\tload: Load a run\n");
		printf("\texit: Close the game\n");
		confirm();
		break;
	//Help pages here
	}
	//printf("Press Enter to go back to the help menu...\n");
}
void input_player(game g)
{
	char input[7] = "";
	fgets(input, 6, stdin);

	if (str_equal(input, "help"))
	{
		bool done = false;
		while (!done)
		{
			char i[4];
			printf("What do you want to know ? (Type a number)\n\n\t1. What's this game ?\n\t2. How to play\n\t3. Advanced commands\n\t4. Get back to the game\n");
			fgets(i, 3, stdin);
			getHelp(atoi(i), &done);
		}

	}
	if (str_equal(input, "hint"))
	{
		//Do a move 
	}
	if (str_equal(input, "exit"))
	{
		exit(EXIT_SUCCESS);
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
				char input2[5];
				printf("Enter the distance for car n°%c :\n", input[0]);
				fgets(input2, 4, stdin);
				input[1] = ' ';
				if (isOperatorSimple(input2[0]))
				{
					input[2] = input2[0];
					input[3] = input2[1];
				}else{
					input[2] = input2[0];
				}
			}
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
	else
	{
		printf("Incorrect input.\n");
	}

}