#include <solver.h>

void randmove(game g){
	//mouvement aléatoire, utilisation de time.h
	time_t t;
	//initialisation du générateur aléatoire
	srand((unsigned) time(&t));
	dir d = rand()%3;
	play_move(g, rand()%game_nb_pieces(g),d,rand()%5);
}

bool solve(game g){
	if (whatGame("rush-hour\n")){
		while(!game_over_hr(g))
			randmove(g);
		return true;
	}
	else{
		//while(!game_over_ar(g))
		//	randmove(g);
		//return true;
	}
	return false;
}