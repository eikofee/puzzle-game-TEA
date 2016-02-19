#include <stdlib.h>
#include <stdio.h>
#include "piece.h"

bool estAlloue(void* p);
void switchsMovePiece(piece p, dir d, int distance);
bool estPositionValide(piece p);
int** pieceEnTableau(piece p, int taille);

int TAILLE_PLATEAU = 6;

struct piece_s{
	bool isHorizontal; // true si la piece est horizontal, false sinon.
	bool isSmall; // true si la taille de la piece est 2, et false si 3.
	int position[2]; // position de la case inférieur gauche. (x;y)
};

piece new_piece_rh (int x, int y, bool small, bool horizontal){
	piece newPiece = (piece)malloc(sizeof(struct piece_s));

	if(newPiece == NULL)
	{
		fprintf(stderr, "Erreur: pas d'allocation mémoire\n");
		exit(EXIT_FAILURE);
	}

	if(x < 0 || x > (TAILLE_PLATEAU - 1) || y < 0 || y >= (TAILLE_PLATEAU - 1))
	{
		fprintf(stderr, "Erreur: x ou y inférieur à 0\n");
		exit(EXIT_FAILURE);
	}

	newPiece -> isHorizontal = horizontal;
	newPiece -> isSmall = small;
	newPiece -> position[0] = x;
	newPiece -> position[1] = y;

	return newPiece;
}

void delete_piece (piece p){
	if(p != NULL)
		free(p);
}

void copy_piece (cpiece src, piece dst){
	if(src == NULL || dst == NULL)
	{
		fprintf(stderr,"Erreur: problème d'allocation mémoire avec src ou dst\n");
		exit(EXIT_FAILURE);
	}
	dst -> isHorizontal = is_horizontal(src);
	dst -> isSmall = src -> isSmall;
	dst -> position[0] = get_x(src);
	dst -> position[1] = get_y(src);
}

//Cette fonction permet de bouger une piece sur une distance donnée
//dans une direction d. On réalise 3 tests pour déterminer si 
//les paramètres sont cohérents et valides.
//On appelle une fonction annexe switchsMovePiece pour faire le mouvement.
void move_piece (piece p, dir d, int distance){
	if(p == NULL)
	{
		fprintf(stderr, "Erreur: pas d'allocation mémoire\n");
		exit(EXIT_FAILURE);
	}

	if(!estPositionValide(p))
	{
		fprintf(stderr, "Erreur: Position de la piece invalide\n");
		exit(EXIT_FAILURE);
	}
	//Simple vérification pour savoir si la direction est cohérente avec l'orientation de la pièce.
	if( (is_horizontal(p) && ((d == UP) || (d == DOWN))) || ( (!(is_horizontal(p))) && ((d == RIGHT) || (d == LEFT))))
	{
		return;
	}

	//A ce stade, tout est ok pour réaliser le mouvement.
	// On vérifie juste si la distance est bien >0
	if(distance > 0)
		switchsMovePiece(p, d, distance);

}

//Vérifie si la position de la piece est bien dans le plateau (6x6)
bool estPositionValide(piece p){
	if((get_x(p) < 0) || (get_y(p) < 0))
		return false;

	//Ici la piece est horizontal
	if(is_horizontal(p))
	{
		if(p -> isSmall) // Ici la piece fait 2 cases
		{
			if(get_x(p) > (TAILLE_PLATEAU - 2) || get_y(p) > (TAILLE_PLATEAU - 1))
				return false;
		}
		else // ici la piece fait 3 cases
		{
			if(get_x(p) > (TAILLE_PLATEAU - 3) || get_y(p) > (TAILLE_PLATEAU - 1))
				return false;
		}
	}
	// ici la piece est a la verticale
	else 
	{
		if(p -> isSmall) //ici la piece fait 2 cases
		{
			if(get_x(p) > (TAILLE_PLATEAU - 1) || get_y(p) > (TAILLE_PLATEAU - 2))
				return false;
		}
		else //ici la piece fait 3 cases
		{ 
			if(get_x(p) > (TAILLE_PLATEAU - 1) || get_y(p) > (TAILLE_PLATEAU - 3))
				return false;
		}
	}
	return true;
}


//Fonction qui effectue le mouvement en passant par une copie (p_copy)
//Ainsi, on vérifie seulement a la fin si la position est valide de la copie
//Et on recopie dans p
void switchsMovePiece(piece p, dir d, int distance){
	int taille_piece = 2;

	if(p -> isSmall)
		taille_piece = 1;

	piece p_copy = new_piece_rh(0,0,true,true);
	copy_piece(p, p_copy);
	
	switch (d){
		case UP:
			if((get_y(p_copy) + distance + taille_piece) <= 5)
				(p_copy -> position[1]) += distance;
			break;

		case DOWN:
			if((get_y(p_copy) - distance) >= 0)
				(p_copy -> position[1]) -= distance;
			break;

		case RIGHT:
			if((get_x(p_copy) + distance + taille_piece) <= 5)
				(p_copy -> position[0]) += distance;
			break;

		case LEFT:
			if((get_x(p_copy) - distance) >= 0)
				(p_copy -> position[0]) =get_x(p_copy) - distance;
			break;

		default:
			printf("Cas non prévu\n");
			break;
	}

	if(estPositionValide(p_copy))
		copy_piece(p_copy, p);

	delete_piece(p_copy);
}

//Retourne true si il deux pièces se chevauchent, false sinon.
//On emploi 2 tableau2D crées par la fonction pieceEnTableau afin de vérifier plus facilement
//les problèmes de collisions.
bool intersect(cpiece p1, cpiece p2){

	int taille_p1 = 3;
	int taille_p2 = 3;

	if(p1 -> isSmall)
		taille_p1 = 2;

	if(p2 -> isSmall)
		taille_p2 = 2;

	int** tab_p1 = pieceEnTableau(p1, taille_p1);
	int** tab_p2 = pieceEnTableau(p2, taille_p2);

	for(int x = 0; x < taille_p1; x++)
	{
		for(int y = 0; y < taille_p2; y++)
		{
			if((tab_p1[x][0] == tab_p2[y][0]) && (tab_p1[x][1] == tab_p2[y][1]))
				return true;
		}
	}
	return false;
}

//Crée et retourne un tableau 2D des coordonnées de chaque case prise par la piece
//Cela nous permet de mieux gérer les conflits entre 2 pieces dans intersect.
//Prend en paramètre une piece et sa taille.
int** pieceEnTableau(piece p, int taille){
	int **tab = (int **)malloc(taille * sizeof(int*));
	int *tab2 =(int *)malloc(taille * 2 * sizeof(int));

	for(int i = 0; i < taille; i++)
		tab[i] = &tab2[i*2];

	if(p -> isHorizontal)
	{ 
		for(int j = 0; j < taille; j++)
		{
			tab[j][0] = (p->position[0]) + j;
			tab[j][1] = p->position[1];
		}
	}
	else
	{
		for(int j = 0; j < taille; j++)
		{
			tab[j][0] = p->position[0];
			tab[j][1] = (p->position[1]) + j;
		}
	}
	return tab;
}

//Génere un tableau 2D correspondant au plateau du jeu
//Il est rempli par la valeur -1 par défaut. Les vehicules sont mis grace
// au tableau de piece.
int** TableauDePieces(piece* tab_pieces, int taille){
	int **tab2Dpieces = (int **) malloc(TAILLE_PLATEAU * sizeof(int*));
	int *tab2Dpieces2 = (int *) malloc(TAILLE_PLATEAU * TAILLE_PLATEAU * sizeof(int));

	int **tab_tmp_piece;
	int taille_tab_tmp_piece;

	for(int i = 0; i < TAILLE_PLATEAU; i++)
		tab2Dpieces[i] = &tab2Dpieces2[i*TAILLE_PLATEAU];

	for(int x = 0; x < TAILLE_PLATEAU; x++)
		for(int y = 0; y < TAILLE_PLATEAU; y++)
			tab2Dpieces[x][y] = -1;

	for(int i = 0; i < taille; i++)
	{
		taille_tab_tmp_piece = 3;
		if(tab_pieces[i] -> isSmall)
			taille_tab_tmp_piece = 2;

		tab_tmp_piece = pieceEnTableau(tab_pieces[i], taille_tab_tmp_piece);
		for(int j = 0; j < taille_tab_tmp_piece; j++)
			tab2Dpieces[ tab_tmp_piece[j][0] ][ tab_tmp_piece[j][1] ] = i;
	}

	return tab2Dpieces;
}

// ---- Fonctions Simples ----
int get_x(cpiece p){
	if(p != NULL)
		return p->position[0];
}

int get_y(cpiece p){
	if(p != NULL)
		return p->position[1];
}

int get_height(cpiece p){
	if(!(p->isHorizontal))
	{
		if(p->isSmall)
			return 2;
		return 3;
	}
	return 1;
}

int get_width(cpiece p){
	if(p->isHorizontal)
	{
		if(p->isSmall)
			return 2;
		return 3;
	}
	return 1;
}

bool is_horizontal(cpiece p){
	if(p != NULL)
		return p->isHorizontal;
}
