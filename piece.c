#include <stdlib.h>
#include <stdio.h>
#include "piece.h"

struct piece_s{
	bool isHorizontal; // true for Horizontal ; false for vertical
	bool isSmall; // Size of the vehicule, true for 2 boxes, false for 3.
	int position[2]; // position of his lower left box. (x;y)
};

piece new_piece_rh (int x, int y, bool small, bool horizontal){
	piece newPiece = (piece)malloc(sizeof(struct piece_s));

	if(!estAlloue(newPiece))
		exit(EXIT_FAILURE);

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
	if(!estAlloue(src) || !estAlloue(dst))
		exit(EXIT_FAILURE);

	if(src == NULL || dst == NULL)
	{
		fprintf(stderr,"Erreur: problème d'allocation mémoire avec src ou dst\n");
		exit(EXIT_FAILURE);
	}
	dst -> isHorizontal = src -> isHorizontal;
	dst -> isSmall = src -> isSmall;
	dst -> position[0] = src -> position[0];
	dst -> position[1] = src -> position[1];
}

void move_piece (piece p, dir d, int distance){
	if(!estAlloue(p))
		exit(EXIT_FAILURE);
	//A ce stade, p est bien alloué.

	if(!estPositionValide(p))
	{
		fprintf(stderr, "Erreur: Position de la piece invalide\n")
		exit(EXIT_FAILURE);
	}
	//A ce stade, p est bien positionnée.
	if( (p->isHorizontal && ((d == UP) || (d == DOWN))) || (!p->isHorizontal && ((d == RIGHT) || (d == LEFT))))
	{
		// fprintf(stderr,"Erreur: direction invalide avec cette pièce\n");
		// exit(EXIT_FAILURE);
		return;
	}
	//A ce stade, les paramètres sont cohérents avec l'orientation de p.

	switchsMovePiece(p, dir, distance);
	
}

//Vérifie si la position de la piece est bien dans le plateau.
bool estPositionValide(piece p){
	if((p -> position[0] < 0) || (p -> position[0] > 5) || (p -> position[1] < 0) || (p -> position[1] > 5))
		return false;
	return true;
}

//Verifie si l'allocation de la piece p s'est bien déroulée.
bool estAlloue(piece p){
	if(p == NULL)
	{
		fprintf(stderr,"Erreur: problème d'allocation mémoire\n");
		return false;
	}
	return true;
}

void switchsMovePiece(piece p, dir d, int distance){
	int taille_piece;
	if(p->isSmall)
		taille_piece = 1;
	else
		taille_piece = 2;

	switch (d){

			case UP:
				if((p -> position[1] + distance + taille_piece) <= 5)
					p -> position[1] += distance;
				break;

			case DOWN:
				if((p -> position[1] - distance) >= 0)
					p -> position[1] -= distance;
				break;

			case RIGHT:
				if((p -> position[0] + distance + taille_piece) <= 5)
					p -> position[0] += distance;
				break;

			case LEFT:
				if((p -> position[0] - distance) >= 0)
					p -> position[0] -= distance;
				break;

			default:
				printf("Cas non prévu\n");
				break;
		}
}