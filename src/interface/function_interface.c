#include <stdio.h>
#include <stdlib.h>
#include <game.h>
#include <utility.h>
#include <function_interface.h>

//Ce fichier sert a alleger le fichier interface_txt en y mettant les fonctions annexes à celle
//présentes dans interface_txt

/*	fonction de DEBUG
	Affiche la liste des pieces de facon lisible
*/
void display_pieces(piece *p, int taille)
{
	for (int i = 0; i < taille; i++)
	{
		printf("Piece [%d] : x = %d, y = %d, ",i ,get_x(p[i]), get_y(p[i]));
		printf("Width = %d | Height = %d, ", get_width(p[i]), get_height(p[i]));
		// if (is_horizontal(p[i]))
		// 	printf("isHorizontal");
		// else
		// 	printf("isVertical");
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
	Change la couleur d'une pièce
*/
void setColorPiece(char c, int id, bool fill)
{
	if (id)
		printf("\x1b[%s%cm", (id % 12 >= 6 && id % 7 + 1 != 1?"10":"4"),getHexa(id % 7 + 1));
	else
		printf("\x1b[101m");

	switch(id % 7 + 1 + (id >= 6 && id % 7 + 1 != 1?100:40))
	{
		case 107:
		case 102:
		case 103:
		case 105:
			printf("\x1b[30m");
			break;
		default:
			printf("\x1b[97m");
	}
	printf("%c \x1b[0m", (fill?c:' '));
}

/*
	Comparaison de deux chaines de caractères (true/false)
*/
bool str_equal(char* a, char* b)
{
	int i = 0;
	while (a[i] != '\0' && b[i] != '\0')
	{
		if (a[i] != b[i])
			return false;
		i++;
	}
	return a[i] == '\0' && b[i] == '\0';
}

/*
	Verifie si le char passé en paramètre est un chiffre et ne dépasse pas max_number
*/
bool isNumber(char s, int max_number)
{
	return ((s >= '0' && s <= '0' + max_number) || (s >= 'a' && s <= '\\' + max_number));
}

/*
	Vérifie que le char* commence bien par un nombre (positif/négatif)
*/
bool isInt(char* s, int* pos)
{
	if (isOperatorSimple(s[*pos]))
	{
		*pos += 1;
		if (!isInt(s, pos))
			return false;
	}
	return isNumber(s[*pos], 9);
}

/*
	Vérifie si le char est une direction (u, d, l, r)
*/
bool isDirection(char s)
{
	return (s == 'u' || s == 'd' || s =='l' || s == 'r');
}

/*
	Verifie si s est un + ou un -
*/
bool isOperatorSimple(char s)
{
	return (s == '+' || s == '-');
}

/*
	Conversion d'un caractere en int (hexa minuscule permis)
*/
int getNumber(char s)
{
	if (s >= '0' && s <= '9')
		return s - '0';
	if (s >= 'a' && s <= 'f')
		return s - 'W';
	return '0';
}

/*
	Conversion d'un int en char (hexa minuscule)
*/
char getHexa(int n)
{
	if (n >= 0 && n < 10)
		return n + '0';
	if (n > 9 && n < 16)
		return n + 'W';
	return '0';
}

/*
	Convertie la direction en input en dir
*/
dir getDirection(char* s, int* pos)
{
	while (s[*pos] != '\n')
	{
		switch(s[*pos])
		{
			case 'u':
				return UP;
				break;

			case 'd':
				return DOWN;
				break;

			case 'l':
				return LEFT;
				break;

			case 'r':
				return RIGHT;
				break;

			default:
				*pos += 1;
				break;
		}

	}

	return UP;
}
/*
	Recupere la direction que doit prendre une piece en fonction du signe entre
*/
// ********************** Fonctions inutiles V2 *******************************
// dir getDirection(piece p, char sign)
// {
// 	if (sign == '+')
// 	{
// 		if (p -> isHorizontal)
// 			return RIGHT;
// 		else{
// 			return UP;
// 		}
// 	}else{
// 		if (p -> isHorizontal)
// 			return LEFT;
// 		else{
// 			return DOWN;
// 		}
// 	}
// }

/*
	Bloque l'affichage tant que la touche Entree n'est pas utilisee
*/
void confirm()
{
	printf("\n\tPress 'Enter' to go back to the help menu...\n");
	int c = 0;
	while (c != '\r' && c != '\n')
		c = getchar();	
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
		while ((overflow = getchar()) != '\n');	//On ignore les caractÃ¨res en overflow
	}
}

//Permet de sauvegarder dans le fichier save.txt la partie.
//On ecrase l'ancienne partie à chaque fois qu'on fait appelle a cette fonction.
/*void saveGameFromId(game g, char* id)
{
	FILE *fichier = NULL;
	fichier = fopen("save.txt", "w");

	if(fichier == NULL)
		error("saveGameFromId(), probleme d'ouverture du fichier");

	fprintf(fichier, "%s\n", id);
	if(game_nb_moves(g) != -1)
		fprintf(fichier, "%d\n", game_nb_moves(g));
	fclose(fichier);
}
*/
//Permet de charger une partir a partir de la save.
//Cette fonction est exclusif a la save.
/*void loadGameFromSave(char* fichier, game g)
{
	FILE* fichier_tmp = NULL;
	fichier_tmp = fopen(fichier, "r");
	
	if(fichier_tmp == NULL)
		error("loadGameFromSave(), probleme d'ouverture du fichier");

	char s[128] = "";
	fgets(s, 128, fichier_tmp);

	game g_tmp = NULL; //getGameFromId(s);
	copy_game(g_tmp, g);

	// *****----- A conserver pour la V3 --------*****
	// fgets(s, 128, fichier_tmp);
	// g->nb_moves = atoi(s);

	fclose(fichier_tmp);
	delete_game(g_tmp);

}
*/
//Permet de charger une partie a partir d'un fichier et d'un numero de level
char* loadGameFromNum(char* fichier, char* num)
{
	if(str_equal(num, "save\n"))
	{

	}
	//num représente le numero du level
	//on commence par obtenir la taille de num
	int taille_num = 0;
	while(num[taille_num] != '\n')
		taille_num++;

	//On convertit le char* num en int (numId)
	int numId = 0;
	for(int j = taille_num - 1; j >= 0; j--)
	{
		numId = numId * 10;
		numId = numId + getNumber(num[j]);
	}

	//On ouvre le fichier (save.txt ou games.txt)
	FILE* fichier_tmp = NULL;
	fichier_tmp = fopen(fichier, "r");

	if(fichier_tmp == NULL)
		error("loadGameFromId(), probleme d'ouverture du fichier");

	//On recupere l'id du niveau
	char* s = (char*)malloc(sizeof(char) * 256);

	int i = 0;
	while(i != numId)
	{
		fgets(s, 256, fichier_tmp);
		i++;
	}
	fclose(fichier_tmp);
	//On ferme le fichier apres avoir recurerer l'information ( l'id du niveau )
	//Et on recopie la chaine de caratere `s` dans `s2` et on retourne s2

	char* s2 = (char*)malloc(sizeof(char) * 256);

	i = 0;
	while(s[i] != '\n')
	{
		s2[i] = s[i];
		i++;
	}
	s2[i] ='\0';

	free(s);
	return s2;
}

//Libere l'allocation d'un tableau 2D
void freeTableau2D(int** tab)
{
	free(tab[0]);
	free(tab);
}

/*
	Inverse la direction voulue
*/
void revertDirection(dir* d)
{
	switch (*d)
	{
		case UP:
			*d = DOWN;
			break;
		case DOWN:
			*d = UP;
			break;
		case LEFT:
			*d = RIGHT;
			break;
		case RIGHT:
			*d = LEFT;
			break;
	}
}

piece* copieTableauPieces(game g){

	int nb_pieces = game_nb_pieces(g);
	piece* pieces = (piece*) malloc(sizeof(piece) * nb_pieces);
	
	if(!pieces)
		error("Allocation new_game -> pieces");

	//... Affectation des valeurs par copie

	for (int i = 0; i < nb_pieces; i++)
	{
		pieces[i] = new_piece( 0, 0, 0, 0, false,  false);

		if (!pieces[i])
			error("Allocation new_game -> pieces[i]");

		copy_piece(game_piece(g, i), pieces[i]);

		if (!pieces[i])
		{
			printf("i=%d\n", i);
			error("Copie de pièce");
		}
	}
	return pieces;
}

void freeTableauDePieces(piece* pieces, int nb_pieces){
	for(int i = 0; i < nb_pieces; i++)
		delete_piece(pieces[i]);
	free(pieces);
}

/*Syntaxe version 2:
		(nb_pieces)n(taille_x)x(taille_y)p(1,2 ou 3)w(width)h(height)x(pos_x)y(pos_y)p(next)
	*/
game getGameFromId(char* id)
{
	int i = 0;
	int nb_pieces = readUntilChar(id, &i);
	i++;
	piece p[nb_pieces];
	int n_piece = 0;	//index de p
	int taille_x = 0;
	int taille_y = 0;
	int state = 1; // 1 = taille_x, 2 = taille_y, 3 = piece
	piece p_tmp = NULL;
	

	while (id[i] != '\0')
	{
		switch(state)
		{
			case 1:
				taille_x = readUntilChar(id, &i);
				break;
			case 2:
				taille_y = readUntilChar(id, &i);
				break;
			case 3:

				p_tmp = getPieceFromId(id, &i);
				p[n_piece] = new_piece(0, 0, 0, 0, false, false);
				copy_piece(p_tmp, p[n_piece]);
				n_piece++;
				state--;
				delete_piece(p_tmp);
				break;
		}
		if (id[i])
			i++;
		state++;
	}
	game g = new_game(taille_x, taille_y, nb_pieces, p);
	for(int i = 0; i < nb_pieces; i++)
		delete_piece(p[i]);
	return g;
}


/*Syntaxe version 2:
		[nb_pieces]n[taille_x]x[taille_y]p[1,2 ou 3]w[width]h[height]x[pos_x]y[pos_y]p[next]
		can_move_x = +1
		can_move_y = +2
	*/
void getIdFromGame(game g, char* id)
{
	int pos = 0;
	int n_piece = 0;
	getCharFromInt(id, &pos, game_nb_pieces(g));
	id[pos] = 'n';
	pos++;
	getCharFromInt(id, &pos, game_width(g));
	id[pos] = 'x';
	pos++;
	getCharFromInt(id, &pos, game_height(g));
	while (n_piece < game_nb_pieces(g))
	{
		id[pos] = 'p';
		pos++;
		id[pos] = '0' + (can_move_x(game_piece(g, n_piece))?1:0) + (can_move_y(game_piece(g, n_piece))?2:0);
		pos++;
		id[pos] = 'w';
		pos++;
		getCharFromInt(id, &pos, get_width(game_piece(g, n_piece)));
		id[pos] = 'h';
		pos++;
		getCharFromInt(id, &pos, get_height(game_piece(g, n_piece)));
		id[pos] = 'x';
		pos++;
		getCharFromInt(id, &pos, get_x(game_piece(g, n_piece)));
		id[pos] = 'y';
		pos++;
		getCharFromInt(id, &pos, get_y(game_piece(g, n_piece)));
		n_piece++;
	}
	id[pos] = '\0';
}


piece getPieceFromId(char* id, int* pos)
{
	int w = 0;
	int h = 0;
	int x = 0;
	int y = 0;
	int type = 0;
	int state = 0; // 0 = type, 1 = w, 2 = h, 3 = x, 4 = y
	while (id[*(pos)] != 'p' && id[*(pos)] != '\0')
	{
		switch(state)
		{
			case 0:
				type = readUntilChar(id, pos);
				break;
			case 1:
				w = readUntilChar(id, pos);
				break;
			case 2:
				h = readUntilChar(id, pos);
				break;
			case 3:
				x = readUntilChar(id, pos);
				break;
			case 4:
				y = readUntilChar(id, pos);
				*(pos) -= 1;
				break;
		}
		state++;
		if (id[*(pos)])
			*(pos) += 1;
	}
	piece p = new_piece(x, y, w, h, ((type >= 2)?true:false), ((type == 1 || type == 3)?true:false));
	return p;
}

int readUntilChar(char* s, int* pos)
{
	int n = 0;
	int multiplier = 1;
	while (s[*pos] == '+' || s[*pos] == '-')
	{
		if (s[*pos] == '-')
			multiplier *= -1;
		*pos += 1;
	}
	while (s[*(pos)] >= '0' && s[*(pos)] <= '9')
	{
		n *= 10;
		n += s[*(pos)] - '0';
		*(pos) += 1;
	}

	return n * multiplier;
}

void getCharFromInt(char* s, int* pos, int data)
{
	if (!data)
	{
		s[*(pos)] = '0';
		*(pos) += 1;
		return;
	}
	int taille = 1;
	int data_calc = data / 10;
	while (data_calc > 0)
	{
		data_calc /= 10;
		taille *= 10;
	}
	while (taille > 0)
	{
		s[*pos] = data / taille + '0';
		*(pos) += 1;
		data -= data / taille * taille;
		taille /= 10;
	}
}


void initFileConfig(char* level_name){

	FILE *fichier = NULL;
	fichier = fopen("../config.ini", "w");

	if(fichier == NULL)
		error("initFileConfig(), probleme de création du fichier");

	if(str_equal(level_name, "rush-hour"))
	{
		fprintf(fichier, "%s\n", "rush-hour");
	}
	else if(str_equal(level_name, "klotski"))
	{
		fprintf(fichier, "%s\n", "klotski");
	}
	else
		fprintf(fichier, "%s\n", level_name);

	fclose(fichier);
}

/*
    Teste l'existence d'un fichier
*/
bool whatGame(char* name)
{
    FILE *file = NULL;
    file = fopen("../config.ini", "r");
    if(file == NULL)
    	error("whatGame(), probleme d'ouverture du fichier config.ini");
    
    char* s = (char*)malloc(sizeof(char) * 128);
    fgets(s, 128, file);
    fclose(file);

    if(str_equal(s, name))
    {
    	free(s);
    	return true;
    }
    free(s);
    return true;
    
    
}

void removeSpaces(char* input)
{
	int i = 0;
	int j = 0;
	while (input[i] != '\0')
	{
		if (input[i] != ' ')
		{
			input[j] = input[i];
			j++;
		}
		i++;
	}
	input[j] = '\0';
}