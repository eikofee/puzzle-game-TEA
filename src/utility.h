#ifndef _UTILITY_H_
#define _UTILITY_H_

struct piece_s{
	bool isHorizontal; // true si la piece est horizontal, false sinon.
	bool isSmall; // true si la taille de la piece est 2, et false si 3.
	bool move_x;
	bool move_y;
	int position[2]; // position de la case inférieur gauche. (x;y)
};

struct game_s{
	piece *pieces;	//malloc nécessaire
	int	nb_moves;
	int nb_pieces;
};

void switchsMovePiece(piece p, dir d, int distance);
bool estPositionValide(piece p);
int** TableauDePieces(piece* tab_pieces, int taille);
int** pieceEnTableau(piece p, int taille);
void error(char* s);

#endif