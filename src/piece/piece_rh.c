#include <stdlib.h>
#include <stdio.h>
#include "piece.h"

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


bool is_horizontal(cpiece p){
	return can_move_x(p) && !can_move_y(p);
}