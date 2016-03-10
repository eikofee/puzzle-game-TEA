#ifndef _INTERFACE_TXT_H_
#define	_INTERFACE_TXT_H_

#include "function_interface.h"

game getGameFromIdAR(char*  id);
void getIdFromGameAR(game g, char* id);
void getIdFromGame(game g, char* id);
void draw_interface(game g, char* id);
void getHelp(int input, bool* done);
void input_player(game g, char* id);

#endif