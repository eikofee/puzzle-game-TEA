#ifndef _GAME_H_
#define _GAME_H_
#include <stdbool.h>
#include "piece.h"

/**
 * @file game.h
 *
 * @brief This file describes the board of a rush-our game, the state of a game and all functions needed to manipulate it.
 **/


/**
 * @struct game_s
 * @brief The state of the game is described by the position of the pieces (i.e. vehicles) on the board and the number of moves since the beginning of the game.
 *
 * The game board is a 6*6 grid. The lower left corner has coordinates (0,0).
 * The piece number 0 represents the "red car" that needs to be moved away from the parking.
 * Its initial position is (0,3).
 * A winning game (state) is when the red car touches the right border of the game board (i.e. when its position is (4,3)).
 *
 * cgame is a pointer toward a constant game.
 * That means that it is not possible to modify the game thanks to this pointer.
 * See also: http://www.geeksforgeeks.org/const-qualifier-in-c/
 * See also this more technical discussion: http://stackoverflow.com/questions/8504411/typedef-pointer-const-weirdness
 *
 **/
typedef struct game_s* game;
typedef const struct game_s* cgame;


/**
 * @brief Create a new game given a starting position defined by a set of pieces.
 * The pieces of the created game are copies of the ones given as argument.
 * The number of moves of the game is set to 0.
 * @param nb_pieces number of pieces g
 * @param pieces array of pieces. The pieces given here are not modified by this function.
 * @return a pointer toward the generated game
 */
game new_game_hr (int nb_pieces, piece *pieces);

/**
 * @brief Destroy the game and free allocated memory
 * @param g the game to destroy
 */
void delete_game (game g);

/**
 * @brief Clone the game src into the game dst.
 * @param src the game to copy
 * @param dst the copied game.
 */
void copy_game (cgame src, game dst);

/**
 * @brief Return the number of pieces on the game board g
*/
int game_nb_pieces(cgame g);


/**
 * @brief Return the piece of g with number bum
 * @param piece_num the num of the piece. This value must be between 0 and game_nb_pieces(g)-1.
*/
cpiece game_piece(cgame g, int piece_num);


/**
 * @brief Test is the game is over (i.e. if the piece 0 has reached the exit).
 * @return true if the piece number 0 has coordinates (4,3)
*/
bool game_over_hr(cgame g);


/**
 * @brief Tries to move the piece of number piece_num in direction d for distance cells.
 *
 * If the move is valid, then corresponding piece is moved, the number of moves of the game is incremented by distance and the returned value is true.
 * If the move is not valid, the game g remains unchanged and the function returns false.
 * A move is valid if:
 *   1) the piece stays in the game board,
 *   2) the direction is compatible is the type of the piece,
 *   3) the piece do not cross any other piece during its movement.
 * @param distance length of the move. This value must be strictly positive.
 * @param piece_num the number of the piece to move. This value must be between 0 and game_nb_pieces(g)-1.
 * @return true if the move is valid, false otherwise.
*/
bool play_move(game g, int piece_num, dir d, int distance);

/**
 * @brief Return the number of moves from the beginning of the game g.
 * Remind that move of a single piece of k cells in a single direction counts for k.
 */
int game_nb_moves(cgame g);



///////////// version 2 /////////////////
game new_game (int width, int height, int nb_pieces, piece *pieces);


/**
 *@brief return the width of the grid
 */
int game_width(cgame g);

/**
 *@brief return the height of the grid
 */
int game_height(cgame g);

/**
 * @brief return the number of then piece located on this square (-1 if no piece is present)
 * @param game
 * @param x-coor of the square
 * @param y-coor of the square
 */
int game_square_piece (game g, int x, int y);
#endif
