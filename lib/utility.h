#ifndef _UTILITY_H_
#define _UTILITY_H_

struct piece_s{
	// bool isHorizontal; // true si la piece est horizontal, false sinon.
	// bool isSmall; // true si la taille de la piece est 2, et false si 3.
	bool move_x;
	bool move_y;
	int width;
	int height;
	int position[2]; // position de la case inférieur gauche. (x;y)
};

struct game_s{
	int width;
	int height;
	piece *pieces;	//malloc nécessaire
	int	nb_moves;
	int nb_pieces;
};

void switchsMovePiece(piece p, dir d, int distance);
int** mapPieces(piece* tab_pieces,int taille, int taille_x, int taille_y);
int** pieceArray(piece p, int taille);
void error(char* s);

#endif