#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "utility.h"
#include "interface_txt.h"
#include <string.h>
/*	DEBUG
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
	Change les majuscules en minuscule
*/
void toLower(char* s)
{
	int i = 0;
	while(s[i] != '\0')
	{
		if (s[i] > 64 && s[i] < 91)
			s[i] += 32;
		i++;
	}
}
/*
	Permet de générer un niveau à partir d'un id (non seed)
*/
game getGameFromId(char* id)
{
	//Syntaxe : Nabca2b2c2a3b3c3
	//N = nombre de voitures
	//a = car type: +1 st horizontale, +2 si grand
	//b = case axe x
	//c = case axe y
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
	id[0] = getHexa(game_nb_pieces(g));
	int indexChar = 1;
	for (int i = 0; i < game_nb_pieces(g); i++)
	{
		id[indexChar] = getHexa((is_horizontal(g -> pieces[i])?1:0) + ((g -> pieces[i] -> isSmall)?0:2));
		id[indexChar + 1] = getHexa(get_x(g -> pieces[i]));
		id[indexChar + 2] = getHexa(get_y(g -> pieces[i]));
		indexChar += 3;
	}
	id[indexChar] = '\0';
}


/*
	Change la couleur d'une pièce
*/
char* setColorPiece(char c, int id, bool fill)
{
	char* s = (char*) malloc(sizeof(char) * 17);
	//strcpy(s, "\x1b[XY;1mC\x1b[0m");
	strcpy(s, "\x1b[4YmC \x1b[0m");
	s[3] = (!id ?'1':getHexa(id % 5 + 2));
	s[5] = (fill?c:' ');
	return s;
}
/*
	Affiche la zone de jeu
*/
void draw_interface(game g, char* seed)
{
	int moves = game_nb_moves(g);
	int** t = TableauDePieces(g -> pieces, g -> nb_pieces);
	bool* toWrite = (bool*) malloc(sizeof(bool) * g -> nb_pieces);
	for (int i = 0; i < g -> nb_pieces; i++)
		toWrite[i] = true;
	printf("############### Rush Hour\n");
	for (int i = 5; i > -1; i--)
	{
		printf("# ");
		for (int j = 0; j < 6; j++)
		{
			if (t[j][i] == -1)
				printf(". ");
			else
			{

				//printf("%c", getHexa(t[j][i]));
				char* s = setColorPiece(getHexa(t[j][i]), t[j][i], (toWrite[t[j][i]]?true:false));
				printf("%s", s);
				free(s);
				toWrite[t[j][i]] = false;
			}
		}
		
		switch(i){
			//let's check if we have to display more informations on the right:
			case 0: 
				printf("# Type 'help' for more informations\n");
				break;
			case 1: //ID/Seed du jeu
				printf("# ID: %s\n", seed);
				break;
			case 3: //Sortie du parking
				printf(">\n");
				break;
			case 4:	//Move num display
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
	free(toWrite);
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
	return ((s >= 48 && s <= 48 + max_number) || (s >= 97 && s <= 92 + max_number));
}

/*
	Vérifie si s est un + ou un -
*/
bool isOperatorSimple(char s)
{
	return (s == '+' || s == '-');
}

/*
	Conversion d'un caractère en int (hexa minuscule permis)
*/
int getNumber(char s)
{
	if (s >= 48 && s <= 58)
		return s - 48;
	if (s >= 97 && s <= 102)
		return s - 87;
}

/*
	Conversion d'un int en char (hexa minuscule)
*/
char getHexa(int n)
{
	if (n >= 0 && n < 10)
		return n + 48;
	if (n > 9 && n < 16)
		return n + 87;
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
	int c;
	while (c != '\r' && c != '\n')
		c = getchar();	
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
	Ignore l'overflow de stdin
*/
void ignoreOverflow(char* input, int taille)
{
	int i = 0;
	while (input[i] != 0 && i < taille)
		i++;
	if(i == taille)
	{
		int overflow;
		while ((overflow = getchar()) != '\n');	//On ignore les caractères en overflow
	}
}

/*
	Permet d'obtenir le mouvement de la pièce entrée dans un premier temps
	(Améliore la lisibilité)
*/
void getSecondInput(char* input)
{
												//Current input is "[N][\n][ ][ ][ ][ ]"
	char input2[5];
	printf("Enter the distance for car n°%c :\n", input[0]);
	fgets(input2, 4, stdin);
	input[1] = ' ';								//Current input is "[N][_][ ][ ][ ][ ]"					
	if (isOperatorSimple(input2[0]))
	{
		input[2] = input2[0];					//Current input is "[N][_][+-][ ][ ][ ]"
		input[3] = input2[1];					//Current input is "[N][_][+-][n][ ][ ]"
	}else{
		input[2] = input2[0];					//Current input is "[N][_][n][ ][ ][ ]"
	}
	ignoreOverflow(input2, 3);
}

/*
	Vérifie si s suit un format correct (un peu à la façon de printf)
*/
bool checkFormat(char* s, char* format)
{
	/*Syntaxe:
		%n : le char est un nombre
		%o : le char est un opérateur simple
	*/
	int i = 0;
	int j = 0;
	while (format[i] != '\0')
	{
		if (format[i] != '%')
		{
			if (s[j] != format[i])
				return false;
		}
		else{
			switch(format[++i])
			{
				case 'n':
					if (!isNumber(s[j], 9))
						return false;
					break;

				case 'o':
					if (!isOperatorSimple(s[j]))
						return false;
					break;
				default:
					error("Syntaxe checkFormat incorrecte.");
					break;
			}
		}
		i++;
		j++;
	}
	return true;
}

/*
	Récupère les commandes du joueur (imparfait)
*/
void input_player(game g)
{
	char input[7] = {0, 0, 0, 0, 0, 0, 0};
	fgets(input, sizeof(input), stdin);
	toLower(input);
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
		/*SYNTAXE :
			0 2 : Avance la voiture rouge de 2 cases vers la droite
			1 -1 : Recule la voiture 1 vers le bas si verticale ou la gauche si horizontale
			2 : Demande un déplacement de la voiture 2 (nouveau scanf)
		*/ 
		if (input[1] == '\n')
			getSecondInput(input);
		if (checkFormat(input, "%n %n"))
		{
			play_move(g, getNumber(input[0]), getDirection(g -> pieces[getNumber(input[0])], '+'), getNumber(input[2]));
		}
		if (checkFormat(input, "%n %o%n"))
		{
			if (input[2] == '+')
				play_move(g, getNumber(input[0]), getDirection(g -> pieces[getNumber(input[0])], '+'), getNumber(input[3]));
			else{
				play_move(g, getNumber(input[0]), getDirection(g -> pieces[getNumber(input[0])], '-'), -1 * getNumber(input[3]));
			}
		}
	}
	else
	{
		if (!correct)
		{
			printf("Incorrect input. Type 'help' for more informations.\n");
			ignoreOverflow(input, 6);
		}
	}
}

/*
	Free un tableau 2D passé en paramètre
*/
void freeTableau2D(int** tab)
{
	free(tab[0]);
	free(tab);
}