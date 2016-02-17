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

	if(x < 0 || y < 0)
	{
		fprintf(stderr,"Erreur: x ou y inférieur à 0\n");
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

	if( (p->isHorizontal && ((d == UP) || (d == DOWN))) || (!p->isHorizontal && ((d == RIGHT) || (d == LEFT))))
	{
		fprintf(stderr,"Erreur: direction invalide avec cette pièce\n");
		exit(EXIT_FAILURE);
	}
	//a finir
}

bool estAlloue(piece p){
	if(p == NULL)
	{
		fprintf(stderr,"Erreur: problème d'allocation mémoire\n");
		return false;
	}
	return true;
}