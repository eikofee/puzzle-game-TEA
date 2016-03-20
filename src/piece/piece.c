#include <stdlib.h>
#include <stdio.h>
#include <piece.h>
#include <utility.h>

struct piece_s{
	bool move_x;
	bool move_y;
	int width;
	int height;
	int position[2]; // position de la case inférieur gauche. (x;y)
};

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
	free(p);
}

void freeTable(int **tableau){
	free(tableau[0]);
	free(tableau);
}

void copy_piece (cpiece src, piece dst){
	if(src == NULL || dst == NULL)
		error("copy_piece, src ou dst n'est pas alloué");

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

	//A ce stade, tout est ok pour réaliser le mouvement.
	if( ((d == UP || d == DOWN) && can_move_y(p)) || ((d == LEFT || d == RIGHT) && can_move_x(p)) )
		switchsMovePiece(p, d, distance);
}


void switchsMovePiece(piece p, dir d, int distance){

	//On crée une copie de la pièce p.
	piece p_copy = new_piece(0, 0, 0, 0, true, true);
	copy_piece((cpiece)p, p_copy);
	
	//On effectue les modifications de la piece sur la copie.
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
	
	//On recopie la copie dans la piece d'origine.
	copy_piece((cpiece)p_copy, p);

	delete_piece(p_copy);
}

//Retourne true si les deux pièces se chevauchent, false sinon.
//On emploi 2 tableau2D crées par la fonction pieceArray afin de vérifier plus facilement
//les problèmes de collisions.
bool intersect(cpiece p1, cpiece p2){
	int taille_p1;
	int taille_p2;

	taille_p1 = get_height(p1) * get_width(p1);
	taille_p2 = get_height(p2) * get_width(p2);

	int** tab_p1 = pieceArray((piece)p1, taille_p1);
	int** tab_p2 = pieceArray((piece)p2, taille_p2);

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
int** pieceArray(piece p, int size){
	int **arr = (int **)malloc(size * sizeof(int*));
	int *arr2 =(int *)malloc(size * 2 * sizeof(int));

	for(int i = 0; i < size; i++)
		arr[i] = &arr2[i*2];

	int i = 0;
	for(int x = get_x(p); x < get_x(p) + get_width(p); x++)
	{
		for(int y = get_y(p); y < get_y(p) + get_height(p); y++)
		{
			arr[i][0] = x;
			arr[i][1] = y;
			i++;
		}
	}

	return arr;
}

//Génere un tableau 2D correspondant au plateau du jeu
//Il est rempli par la valeur -1 par défaut. Les vehicules sont mis grace
// au tableau de piece.
int** mapPieces(piece* array_p, int size, int size_x, int size_y){

	// ---	Début de l'allocation ---
	int **arr2Dpieces = (int **) malloc(size_x * sizeof(int*));
	int *arr2Dpieces2 = (int *) malloc(size_x * size_y * sizeof(int));

	if(arr2Dpieces == NULL)
		error("mapPieces(), Problème d'allocation sur arr2Dpieces");
	if(arr2Dpieces2 == NULL)
		error("mapPieces(), Problème d'allocation sur arr2Dpieces2");

	for(int i = 0; i < size_x; i++)
			arr2Dpieces[i] = &arr2Dpieces2[i*size_y];
	// --- Fin de l'allocation ---
	// On rempli le tableau de -1
	for(int x = 0; x < size_x; x++)
		for(int y = 0; y < size_y; y++)
			arr2Dpieces[x][y] = -1;

	// Tableau temporaire
	int **arr_tmp_piece;
	int size_arr_tmp_piece;

	for(int i = 0; i < size; i++)
	{
		size_arr_tmp_piece = get_width(array_p[i]) * get_height(array_p[i]);

		arr_tmp_piece = pieceArray(array_p[i], size_arr_tmp_piece);
		
		for(int j = 0; j < size_arr_tmp_piece; j++)
			arr2Dpieces[ arr_tmp_piece[j][0] ][ arr_tmp_piece[j][1] ] = i;
		free(arr_tmp_piece[0]);
		free(arr_tmp_piece);
	}

	return arr2Dpieces;
}

// ---- Fonctions Simples ----
int get_x(cpiece p){
	if(p != NULL)
		return p->position[0];
	error("get_x(), p n'est pas alloué");
	return -1;
}

int get_y(cpiece p){
	if(p != NULL)
		return p->position[1];
	error("get_y(), p n'est pas alloué");
	return -1;
}

int get_height(cpiece p){
	return p -> height;
}

int get_width(cpiece p){
	return p -> width;
}

bool can_move_x(cpiece p){
	if(p != NULL)
		return p -> move_x;
	error("can_move_x(), la piece p n'est pas alloue");
	return false;
}

bool can_move_y(cpiece p){
	if(p != NULL)
		return p -> move_y;
	error("can_move_y(), la piece p n'est pas alloue");
	return false;
}
