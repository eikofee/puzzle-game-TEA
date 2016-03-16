#ifndef _GAME_RH_H_
#define _GAME_RH_H_


#include <game.h>
#include <piece_rh.h>
#include <function_interface.h>

game new_game_hr( int nb_pieces, piece *pieces);
bool game_over_hr(cgame g);
game getGameFromIdRH(char* id);

#endif