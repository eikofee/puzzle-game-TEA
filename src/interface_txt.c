#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "utility.h"
#include "interface_txt.h"

/*
	Affiche la liste des pièces de façon lisible
*/
void display_pieces(piece *p, int taille)
{
	for (int i = 0; i < taille; i++)
	{
		printf("Pièce [%d] : x = %d, y = %d, ",i ,get_x(p[i]), get_y(p[i]));
		if (p[i] -> isSmall)
			printf("isSmall, ");
		if (p[i] -> isHorizontal)
			printf("isHorizontal");
		printf("]\n");
	}
}

/*
	Permet de générer un niveau à partir d'un id
*/
game getGameFromId(char* id)
{
	//Syntaxe : Nabca2b2c2a3b3c3
	//N = nombre de voitures
	//a = car type: +1 st horizontale, +2 si grand
	//b = case axe x
	//c = case axe y
	printf("id du niveau : %s\n", id);
	int nb_pieces = getNumber(id[0]) ;
	piece p[nb_pieces];
	int i = 1;
	int indexP = 0;

	while (id[i] != '\0')
	{
		bool isHorizontal = (id[i] == '1' || id[i] == '3');
		bool isSmall = (id[i] == '0' || id[i] == '1');
		int x = getNumber(id[i + 1]);
		int y = getNumber(id[i + 2]);
		p[indexP] = new_piece_rh(x, y, isSmall, isHorizontal);
		i += 3;
		indexP++;
	}
	game g = new_game_hr(nb_pieces, p);

	for(int j = 0; j < indexP; j++)
		free(p[j]);

	return g;
}

/*
	Permet de convertir un game en id (pas une sauvegarde complète)
*/
void getIdFromGame(game g, char* id)
{
	id[0] = 48 + game_nb_pieces(g);
	int indexChar = 1;
	for (int i = 0; i < game_nb_pieces(g); i++)
	{
		id[indexChar] = 48 + (is_horizontal(g -> pieces[i])?1:0) + ((g -> pieces[i] -> isSmall)?0:2);
		id[indexChar + 1] = 48 + get_x(g -> pieces[i]);
		id[indexChar + 2] = 48 + get_y(g -> pieces[i]);
		indexChar += 3;
	}
	id[indexChar] = '\0';
}

/*
	Affiche la zone de jeu
*/
void draw_interface(game g, char* id)
{
	int moves = game_nb_moves(g);
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
			case 0: 
				printf("# Type 'help' for more informations\n");
				break;
			case 1:	//Level's id display (optional)
				printf("# ID: %s\n", id);
				//printf("# TEST_VERSION\n");
				break;
			case 3: //Exit
				printf(">\n");
				break;
			case 4:	//Move num display
				//printf("# Move %d / %d\n", moves, num_moves);
				printf("# Move %d\n", moves);
				break;
			default:
				printf("#\n");
				break;
		}
	}
	printf("###############\n");
	printf("Enter the car's number you want to move :\n");
	freeTableau2D(t);
}

/*
	Comparaison de deux chaines de caractères (true/false)
*/
bool str_equal(char* a, char* b)
{
	int i = 0;
	while (a[i] != '\0')
	{
		if (a[i] != b[i])
			return false;
		i++;
	}
	return true;
}

/*
	Vérifie si le char passé en paramètre est un chiffre et ne dépasse pas max_number
*/
bool isNumber(char s, int max_number)
{
	return (s >= 48 && s <= 48 + max_number);
}

/*
	Vérifie si s est un + ou un -
*/
bool isOperatorSimple(char s)
{
	return (s == '+' || s == '-');
}

/*
	Convertion d'un caractère en int
*/
int getNumber(char s)
{
	return s - 48;
}

/*
	Récupère la direction que doit prendre une pièce en fonction du signe entré
*/
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

/*
	Bloque l'affichage tant que la touche Entrée n'est pas utilisée
*/
void confirm()
{
	printf("\n\tPress 'Enter' to go back to the help menu");
	int cha;
	while (cha != '\r' && cha != '\n')
		cha = getchar();	
}

/*
	Affiche l'interface d'aide du jeu
*/
void getHelp(int input, bool* done)
{
	switch(input)
	{
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
			printf("\t[WIP]hint: Get closer to the end by cheating\n");
			printf("\t[WIP]skip: skip the current game for another\n");
			printf("\t[WIP]save: Save the current run for later\n");
			printf("\t[WIP]load: Load a run\n");
			printf("\texit: Close the game\n");
			confirm();
			break;
		case 4:
			*(done) = true;
			break;
	}
}

/*
	Récupère les commandes du joueur (imparfait)
*/
void input_player(game g, char* id)
{
	char input[7] = "";
	fgets(input, 6, stdin);
	bool correct = false;

	if (str_equal(input, "help\n"))
	{
		correct = true;
		bool done = false;
		while (!done)
		{
			char i[4];
			printf("What do you want to know ? (Type a number)\n\n\t1. What's this game ?\n\t2. How to play\n\t3. Advanced commands\n\t4. Get back to the game\n");
			fgets(i, 3, stdin);
			getHelp(getNumber(i[0]), &done);
		}

	}
	if (str_equal(input, "hint\n"))
	{
		correct = true;
		printf("hint [WIP]\n");
		//Do a move 
	}
	if (str_equal(input, "exit\n"))
	{
		correct = true;
		free(id);
		delete_game(g);
		exit(EXIT_SUCCESS);
	}
	if (str_equal(input, "save\n"))
	{
		correct = true;
		//sauvegarde
	}
	
	if (isNumber(input[0], g -> nb_pieces - 1))
	{
		correct = true;
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
				play_move(g, getNumber(input[0]), getDirection(g -> pieces[getNumber(input[0])], '+'), getNumber(input[2]));
			}else{
				if (isOperatorSimple(input[2]))
				{
					if (input[2] == '+')
						play_move(g, getNumber(input[0]), getDirection(g -> pieces[getNumber(input[0])], '+'), getNumber(input[3]));
					else{
						play_move(g, getNumber(input[0]), getDirection(g -> pieces[getNumber(input[0])], '-'), -1 * getNumber(input[3]));
					}
				}
			}
		}
	else
	{
		if (!correct)
			printf("Incorrect input.\n");
	}
}


void freeTableau2D(int** tab)
{
	free(tab[0]);
	free(tab);
}