#include <solver.h>
dir revertDirection2(dir d){
	switch(d){
		case UP: return DOWN;
		case DOWN: return UP;
		case LEFT: return RIGHT;
		case RIGHT: return LEFT;
		default: break;
	}
}

void randmove(game g){
	//mouvement aléatoire, utilisation de time.h
	time_t t;
	//initialisation du générateur aléatoire
	srand((unsigned) time(&t));
	dir d;
	int index_p = rand()%game_nb_pieces(g);
	if (can_move_x(game_piece(g,index_p))){
		int direction_value = rand()%1;
		if (direction_value == 0)
			d = LEFT;
		else
			d = RIGHT;
	}
	if (can_move_y(game_piece(g,index_p))){
		int direction_value = rand()%1;
		if (direction_value == 0)
			d = UP;
		else
			d = DOWN;
	}
	int distance = rand()%(2-1)+1;
	if (!play_move(g, index_p,d,distance))
		play_move(g, index_p,revertDirection2(d),distance);
}

bool solve(game g, char* id){
	//mouvement aléatoire, utilisation de time.h
	time_t t;
	//initialisation du générateur aléatoire
	srand((unsigned) time(&t));

	if (whatGame("rush-hour\n")){
		while(!game_over_hr(g)){
			// randmove(g);
			// if (!play_move(g, rand()%4, rand()%4, 1))
			play_move(g, rand()%game_nb_pieces(g), rand()%4, 1);
			// getIdFromGame(g,id);
			// drawInterface(g,id);

		}
		return true;
	}
	else{
		// while(!game_over_ar(g))
			// play_move(g, rand()%game_nb_pieces(g), rand()%4, 1);
			// randmove(g);
		return true;
	}
	return false;
}

/* REFONTE */


// bool solve(game g, char* id){
	
// }