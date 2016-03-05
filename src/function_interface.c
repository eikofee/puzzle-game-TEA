#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "utility.h"
#include "function_interface.h"

/*	DEBUG
	Affiche la liste des pieces de facon lisible
*/
void display_pieces(piece *p, int taille)
{
	for (int i = 0; i < taille; i++)
	{
		printf("Piece [%d] : x = %d, y = %d, ",i ,get_x(p[i]), get_y(p[i]));
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
	while (a[i] != '\0')
	{
		if (a[i] != b[i])
			return false;
		i++;
	}
	return true;
}

/*
	Verifie si le char passé en paramètre est un chiffre et ne dépasse pas max_number
*/
bool isNumber(char s, int max_number)
{
	return ((s >= 48 && s <= 48 + max_number) || (s >= 97 && s <= 92 + max_number));
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
	Recupere la direction que doit prendre une piece en fonction du signe entre
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
	Bloque l'affichage tant que la touche Entree n'est pas utilisee
*/
void confirm()
{
	printf("\n\tPress 'Enter' to go back to the help menu");
	int c;
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
void saveGameFromId(game g, char* id)
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

//Permet de charger une partir a partir de la save.
//Cette fonction est exclusif a la save.
void loadGameFromSave(FILE* fichier, game g)
{
	FILE* fichier_tmp = NULL;
	fichier_tmp = fopen(fichier, "r");
	
	if(fichier_tmp == NULL)
		error("loadGameFromSave(), probleme d'ouverture du fichier");

	char s[128] = "";
	fgets(s, 128, fichier_tmp);

	game g_tmp = getGameFromId(s);
	copy_game(g_tmp, g);

	fgets(s, 128, fichier_tmp);
	g->nb_moves = atoi(s);

	fclose(fichier_tmp);
	delete_game(g_tmp);

}

//Permet de charger une partie a partir d'un fichier et d'un numero de level
char* loadGameFromNum(FILE* fichier, char* num)
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
	char* s = (char*)malloc(sizeof(char) * 128);

	int i = 0;
	while(i != numId)
	{
		fgets(s, 128, fichier_tmp);
		i++;
	}
	fclose(fichier_tmp);
	//On ferme le fichier apres avoir recurerer l'information ( l'id du niveau )
	//Et on recopie la chaine de caratere `s` dans `s2` et on retourne s2

	char* s2 = (char*)malloc(sizeof(char) * 128);

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