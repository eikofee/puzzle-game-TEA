#ifndef _SOLVER_H_
#define _SOLVER_H_

#include <stdlib.h>
#include <time.h>
#include <game.h>
#include <game_ar.h>
#include <game_rh.h>
#include <interface_txt.h>
#include <utility.h>


void randmove(game g);

bool solve(game g, char* id);

dir revertDirection2(dir d);

#endif
