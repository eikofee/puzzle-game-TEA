#ifndef _UTILITY_H_
#define _UTILITY_H_

#include "game.h"
#include <stdlib.h>
#include <stdio.h>

void switchsMovePiece(piece p, dir d, int distance);
bool estPositionValide(piece p);
int** TableauDePieces(piece* tab_pieces, int taille);
int** pieceEnTableau(piece p, int taille);
void error(char* s);

#endif