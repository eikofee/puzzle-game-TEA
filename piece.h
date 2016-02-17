#ifndef _PIECE_H_
#define _PIECE_H_
#include <stdbool.h> //The type bool is defined in this file


/**
 * @file piece.h
 *
 * @brief A piece represents a vehicle placed on the board of the game rush-hour.
 *
 **/

/**
 * @struct piece_s
 * @brief A piece represents a vehicle of the game rush hour placed on the board.
 *
 * A piece is characterized by its position, its shape and its ability to move vertically or horizontally.
 * piece is a pointer to the structure that describes a vehicle on the board.
 *
 * cpiece is a pointer toward a constant piece.
 * This means that it is not possible to modify the piece through to this pointer.
 * See also: http://www.geeksforgeeks.org/const-qualifier-in-c/
 * See also this more technical discussion: http://stackoverflow.com/questions/8504411/typedef-pointer-const-weirdness
 *
 */
typedef struct piece_s* piece;
typedef const struct piece_s* cpiece;

/**
 * @enum dir_e
 * @brief The enumeration of possible directions of a move
 */
typedef enum dir_e {UP, LEFT, DOWN, RIGHT} dir;

/**
 * @brief Creates a new piece.
 * @param x,y coordinates of the lower-left corner (Remark: x-axe is oriented from bottom to top and y-axe is oriented from left to right).
 * @param small indicates if the vehicle is of size 2 (small=true) or 3 (small=false)
 * @param horizontal indicates whether the vehicle can move horizontally or vertically
 * @return a pointer toward the generated piece
 */
piece new_piece_rh (int x, int y, bool small, bool horizontal);


/**
 * @brief Destroys the piece and frees allocated memory
 * @param p the piece to destroy
 */
void delete_piece (piece p);

/**
 * @brief Clones the piece src into the piece dst.
 * @param[out] src the piece to copy
 * @param[in]  dst the copied piece
 */
void copy_piece (cpiece src, piece dst);

/**
 *
 * @brief Tries to move the piece p in direction d with distance steps.
 * If the direction is LEFT or RIGHT and the piece is not allowed to move horizontally, then the piece is not moved.
 * Similarly, if the direction is UP or DOWN and the piece is not allowed to move vertically, then the piece is not moved.
 *
 */
void move_piece (piece p, dir d, int distance);

/**
 * @brief tests whether or not pieces p1 and p2 overlap.
 * @return true if pieces p1 and p2 have at least one cell in common and false otherwise.
 */
bool intersect(cpiece p1, cpiece p2);

/**
 * @brief Returns the abscissa of leftmost point of the piece p.
 */
int get_x(cpiece p);

/**
 * @brief Returns the ordinate of lowest point of the piece p.
 */
int get_y(cpiece p);

/**
 * @brief Returns the different of ordinates of the uppermost and the lowest points of the piece p.
 */
int get_height(cpiece p);

/**
 * @brief Returns the different of abscissas of the leftmost and the rightmost points of the piece p.
 */
int get_width(cpiece p);

/**
 * @brief Returns true if the piece is
 */
bool is_horizontal(cpiece p);

#endif
