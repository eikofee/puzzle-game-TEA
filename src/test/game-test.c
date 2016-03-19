#include <stdio.h>
#include <stdlib.h>
#include <game.h>
#include <utility.h>

//Tiré de test_piece1.c 
bool test_equality_bool(bool expected, bool value, char * msg) {
  if (expected != value)
    fprintf(stderr,"ERR: value expected %d ; value computed %d. %s\n", expected, value, msg);
  return expected == value;
}
//Tiré de test_piece1.c 
bool test_equality_int(int expected, int value, char * msg) {
  if (expected != value)
    fprintf(stderr,"ERR: value expected %d ; value computed %d. %s\n", expected, value, msg);
  return expected == value;
}

game testNewGame(piece *p, int nb_pieces){
	game g = new_game(6,6,nb_pieces,p);
	if (g == NULL)
		exit(EXIT_FAILURE);
	return g;
}
game testCopyGame(game g, game dst){
	copy_game((cgame) g, dst);
	return dst;
}
bool testDeleteGame(game g){
	if(g == NULL)
		return false;
	delete_game(g);
	return true;
}
int main(int argc, char* argv[])
{
	bool result = true;
	int nb_pieces = 4;
	piece p[nb_pieces];
	p[0] = new_piece_rh(0, 3, true, true);		//Voiture rouge
	p[1] = new_piece_rh(2, 2, false, false);	//Camion jaune sur l'ex
	p[2] = new_piece_rh(5, 2, true, false);		//Voiture verte sur l'ex
	p[3] = new_piece_rh(0, 0, false, true);		//Camion horizontal en 0,0
	game g = testNewGame(p, nb_pieces);
	result = result && test_equality_bool(true, g != NULL && game_nb_pieces(g) == nb_pieces && game_piece(g,2) != NULL, "new_game et game_piece");
	result = result && test_equality_int(game_width(g), 6, "newgame et game_width");
	result = result && test_equality_int(game_height(g), 6, "new_game et game_height");
	//création d'un jeu vide
	game g2 = new_game(0,0,0,NULL);
	//puis copie de g dans g2
	testCopyGame(g,g2);
	result = result && test_equality_bool(true, g2 != NULL && game_nb_pieces(g) == game_nb_pieces(g2), "copy_game");
	result = result && test_equality_int(game_nb_pieces(g), nb_pieces, "game_nb_pieces");
	result = result && test_equality_bool(false, play_move(g, 0, LEFT, 1), "play_move");
	result = result && test_equality_bool(true, play_move(g, 1, UP, 1), "play_move");
	result = result && test_equality_bool(true, play_move(g, 2, DOWN, 1), "play_move");
	result = result && test_equality_bool(true, play_move(g, 3, RIGHT, 1), "play_move");
	result = result && test_equality_bool(true, testDeleteGame(g2), "testDeleteGame");
	result = result && test_equality_int(0, game_square_piece(g, 0, 3), "game_square_piece");
	delete_game(g);
	delete_piece(p[0]);
	delete_piece(p[1]);
	delete_piece(p[2]);
	delete_piece(p[3]);

	if(result){
		printf("Good.");
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

