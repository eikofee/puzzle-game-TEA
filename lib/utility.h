#ifndef _UTILITY_H_
#define _UTILITY_H_




void switchsMovePiece(piece p, dir d, int distance);
int** mapPieces(piece* tab_pieces,int taille, int taille_x, int taille_y);
int** pieceArray(piece p, int taille);
void error(char* s);

#endif