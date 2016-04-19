#ifndef _SOLVER_H_
#define _SOLVER_H_

#include <stdlib.h>
#include <time.h>
#include <game_ar.h>
#include <game_rh.h>
#include <interface_txt.h>
#include <utility.h>
typedef enum mode_e {OPTI_MOVES, HINT} mode;
void solve(game g, bool rh, mode cmd);


#endif
