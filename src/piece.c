#include <stdlib.h>
#include <stdio.h>
#include "piece.h"
#include "utility.h"

//variable globale pour la taille du plateau
int TAILLE_PLATEAU = 6;

piece new_piece_rh (int x, int y, bool small, bool horizontal){

	int width;
	int height;

	if(horizontal)
	{
		height = 1;
		width = small ? 2 : 3;
	}
	else
	{
		height = small ? 2 : 3;
		width = 1;
	}

	piece newPiece = new_piece(x, y, width, height, horizontal, !horizontal);
	
	return newPiece;
}


piece new_piece (int x, int y, int width, int height, bool move_x, bool move_y){
	piece newPiece = (piece)malloc(sizeof(struct piece_s));
	
	if(newPiece == NULL)
		error("new_piece(), Probleme d'allocation mémoire");

	newPiece -> position[0] = x;
	newPiece -> position[1] = y;
	newPiece -> width = width;
	newPiece -> height = height;
	newPiece -> move_x = move_x;
	newPiece -> move_y = move_y;

	return newPiece;
}


void delete_piece (piece p){
	if(p != NULL)
		free(p);
}

void freeTable(int **tableau){
	free(tableau[0]);
	free(tableau);
}

void copy_piece (cpiece src, piece dst){
	if(src == NULL || dst == NULL)
		error("copy_piece, src ou dst n'est pas alloué");

	// dst -> isHorizontal = is_horizontal(src);
	// dst -> isSmall = src -> isSmall;
	dst -> position[0] = get_x(src);
	dst -> position[1] = get_y(src);
	dst -> width = get_width(src);
	dst -> height = get_height(src);
	dst -> move_x = can_move_x(src);
	dst -> move_y = can_move_y(src);
}

//Cette fonction permet de bouger une piece sur une distance donnée
//dans une direction d. On réalise 3 tests pour déterminer si 
//les paramètres sont cohérents et valides.
//On appelle une fonction annexe switchsMovePiece pour faire le mouvement.
void move_piece (piece p, dir d, int distance){
	if(p == NULL)
		error("move_piece, p n'est pas alloué");

	//Simple vérification pour savoir si la direction est cohérente avec l'orientation de la pièce.
	// if( (is_horizontal(p) && ((d == UP) || (d == DOWN))) || ( (!(is_horizontal(p))) && ((d == RIGHT) || (d == LEFT))))
	// {
	// 	return;
	// }

	//A ce stade, tout est ok pour réaliser le mouvement.
	if( ((d == UP || d == DOWN) && can_move_y(p)) || ((d == LEFT || d == RIGHT) && can_move_x(p)) )
		switchsMovePiece(p, d, distance);
	

}


//Fonction qui effectue le mouvement en passant par une copie (p_copy)
//Ainsi, on vérifie seulement a la fin si la position est valide de la copie
//Et on recopie dans p
void switchsMovePiece(piece p, dir d, int distance){

	piece p_copy = new_piece(0, 0, 0, 0, true, true);
	copy_piece(p, p_copy);
	
	switch (d){
		case UP:
			(p_copy -> position[1]) += distance;
			break;

		case DOWN:
			(p_copy -> position[1]) -= distance;
			break;

		case RIGHT:
			(p_copy -> position[0]) += distance;
			break;

		case LEFT:
			(p_copy -> position[0]) -= distance;
			break;

		default:
			printf("Cas non prévu\n");
			break;
	}
	
	copy_piece(p_copy, p);

	delete_piece(p_copy);
}

//Retourne true si les deux pièces se chevauchent, false sinon.
//On emploi 2 tableau2D crées par la fonction pieceEnTableau afin de vérifier plus facilement
//les problèmes de collisions.
bool intersect(cpiece p1, cpiece p2){
	int taille_p1;
	int taille_p2;

	taille_p1 = get_height(p1) * get_width(p1);
	taille_p2 = get_height(p2) * get_width(p2);

	int** tab_p1 = pieceEnTableau(p1, taille_p1);
	int** tab_p2 = pieceEnTableau(p2, taille_p2);

	for(int x = 0; x < taille_p1; x++)
	{
		for(int y = 0; y < taille_p2; y++)
		{
			if((tab_p1[x][0] ==tab_p2[y][0]) && (tab_p1[x][1] == tab_p2[y][1]))
			{
				freeTable(tab_p1);
				freeTable(tab_p2);
				return true;
			}
		}
	}
	freeTable(tab_p1);
	freeTable(tab_p2);
	return false;
}

//Crée et retourne un tableau 2D des coordonnées de chaque case prise par la piece
//Cela nous permet de mieux gérer les conflits entre 2 pieces dans intersect.
//Prend en paramètre une piece et sa taille.
int** pieceEnTableau(piece p, int taille){
	// int taille = get_height(p) * get_width(p);
	int **tab = (int **)malloc(taille * sizeof(int*));
	int *tab2 =(int *)malloc(taille * 2 * sizeof(int));

	for(int i = 0; i < taille; i++)
		tab[i] = &tab2[i*2];

	// if(p -> isHorizontal)
	// { 
	// 	for(int j = 0; j < taille; j++)
	// 	{
	// 		tab[j][0] = (p->position[0]) + j;
	// 		tab[j][1] = p->position[1];
	// 	}
	// }
	// else
	// {
	// 	for(int j = 0; j < taille; j++)
	// 	{
	// 		tab[j][0] = p->position[0];
	// 		tab[j][1] = (p->position[1]) + j;
	// 	}
	// }
	// for(int x = 0; x < get_width(p); x++)
	// {
	// 	for(int y = 0; y < get_height(p); y++)
	// 	{
	// 		tab[x][0] = get_x(p) + x;
	// 		tab[x][0] = get_y(p) + y + x;
	// 	}
	// }

	// for(int i = 0; i < taille; i++)
	// {
	int i = 0;
	for(int x = get_x(p); x < get_x(p) + get_width(p); x++)
	{
		for(int y = get_y(p); y < get_y(p) + get_height(p); y++)
		{
			tab[i][0] = x;
			tab[i][1] = y;
			i++;
		}
	}
	// }

	return tab;
}

//Génere un tableau 2D correspondant au plateau du jeu
//Il est rempli par la valeur -1 par défaut. Les vehicules sont mis grace
// au tableau de piece.
int** TableauDePieces(piece* tab_pieces, int taille_x, int taille_y){

	// ---	Début de l'allocation ---
	int **tab2Dpieces = (int **) malloc(taille_x * sizeof(int*));
	int *tab2Dpieces2 = (int *) malloc(taille_x * taille_y * sizeof(int));

	if(tab2Dpieces == NULL)
		error("TableauDePieces, Problème d'allocation sur tab2Dpieces");
	if(tab2Dpieces2 == NULL)
		error("TableauDePieces, Problème d'allocation sur tab2Dpieces2");

	for(int i = 0; i < taille_x; i++)
			tab2Dpieces[i] = &tab2Dpieces2[i*taille_y];
	// --- Fin de l'allocation ---
	// On rempli le tableau de -1
	for(int x = 0; x < taille_x; x++)
		for(int y = 0; y < taille_y; y++)
			tab2Dpieces[x][y] = -1;

	// Tableau temporaire
	int **tab_tmp_piece;
	int taille_tab_tmp_piece;

	for(int i = 0; i < taille; i++)
	{
		taille_tab_tmp_piece = get_width(tab_pieces[i]) * get_height(tab_pieces[i]);

		tab_tmp_piece = pieceEnTableau(tab_pieces[i], taille_tab_tmp_piece);
		
		for(int j = 0; j < taille_tab_tmp_piece; j++)
			tab2Dpieces[ tab_tmp_piece[j][0] ][ tab_tmp_piece[j][1] ] = i;
		free(tab_tmp_piece[0]);
		free(tab_tmp_piece);
	}

	return tab2Dpieces;
}

// ---- Fonctions Simples ----
int get_x(cpiece p){
	if(p != NULL)
		return p->position[0];
	error("get_x(), p n'est pas alloué");
}

int get_y(cpiece p){
	if(p != NULL)
		return p->position[1];
	error("get_y(), p n'est pas alloué");
}

int get_height(cpiece p){
	return p -> height;
}

int get_width(cpiece p){
	return p -> width;
}

bool is_horizontal(cpiece p){
	return can_move_x(p);
}

bool can_move_x(cpiece p){
	if(p != NULL)
		return p -> move_x;
	error("can_move_x(), la piece p n'est pas alloue");
}

bool can_move_y(cpiece p){
	if(p != NULL)
		return p -> move_y;
	error("can_move_y(), la piece p n'est pas alloue");
}
