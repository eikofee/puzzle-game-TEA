#include <stdlib.h>
#include <stdio.h>
#include "piece.h"

bool estAlloue(void* p);
void switchsMovePiece(piece p, dir d, int distance);
bool estPositionValide(piece p);
int** pieceEnTableau(piece p, int taille);

struct piece_s{
	bool isHorizontal; // true for Horizontal ; false for vertical
	bool isSmall; // Size of the vehicule, true for 2 boxes, false for 3.
	int position[2]; // position of his lower left box. (x;y)
};

piece new_piece_rh (int x, int y, bool small, bool horizontal){
	piece newPiece = (piece)malloc(sizeof(struct piece_s));

	if(newPiece == NULL)
	{
		fprintf(stderr, "Erreur: pas d'allocation mémoire\n");
		exit(EXIT_FAILURE);
	}

	if(x < 0 || x > 5 || y < 0 || y > 5)
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
	// dst -> isHorizontal = src -> isHorizontal;
	// dst -> isSmall = src -> isSmall;
	// dst -> position[0] = src -> position[0];
	// dst -> position[1] = src -> position[1];
	dst = new_piece_rh(get_x(src), get_y(src), src -> isSmall, is_horizontal(src));
}

void move_piece (piece p, dir d, int distance){
	if(p == NULL)
	{
		fprintf(stderr, "Erreur: pas d'allocation mémoire\n");
		exit(EXIT_FAILURE);
	}
	//A ce stade, p est bien alloué.

	if(!estPositionValide(p))
	{
		fprintf(stderr, "Erreur: Position de la piece invalide\n");
		exit(EXIT_FAILURE);
	}
	//A ce stade, p est bien positionnée.

	if( (p -> isHorizontal && ((d == UP) || (d == DOWN))) || ((!(p -> isHorizontal)) && ((d == RIGHT) || (d == LEFT))))
	{
		//fprintf(stderr,"Erreur: direction invalide avec cette pièce\n");
		return;
	}
	//A ce stade, les paramètres sont cohérents avec l'orientation de p.

	if(distance > 0)
		switchsMovePiece(p, d, distance);

}

//Vérifie si la position de la piece est bien dans le plateau.
bool estPositionValide(piece p){
	if((p -> position[0] < 0) || (p -> position[1] < 0))
		return false;

	//dans le cas de la piece horizontal :
	if(p -> isHorizontal)
	{
		if(p -> isSmall) // Ici la piece fait 2 cases
			if(p -> position[0] > 4 || p -> position[1] > 5)
				return false;
		else // ici la piece fait 3 cases
			if(p -> position[0] > 3 || p -> position[1] > 5)
				return false;
	}
	else // ici la piece est a la verticale
	{
		if(p -> isSmall) //ici la piece fait 2 cases
			if(p -> position[0] > 5 || p -> position[1] > 4)
				return false;
		else //ici la piece fait 3 cases
			if(p -> position[0] > 5 || p -> position[1] > 3)
				return false;
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

	piece p_copy = (piece)malloc(sizeof(struct piece_s));
	if(p_copy == NULL)
	{
		fprintf(stderr, "Erreur: problème d'allocation mémoire\n");
		exit(EXIT_FAILURE);
	}

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
				(p_copy -> position[0]) -= distance;
			break;

		default:
			printf("Cas non prévu\n");
			break;
	}

	if(estPositionValide(p_copy)){
		copy_piece(p_copy, p);
	}

	delete_piece(p_copy);
}

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

int** pieceEnTableau(piece p, int taille){

	int **tab = (int **)malloc(taille * sizeof(int*));
	int *tab2 =(int *)malloc(sizeof(int)*taille*2);
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
