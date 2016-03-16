#ifndef _INTERFACE_TXT_H_
#define	_INTERFACE_TXT_H_

#include "function_interface.h"

void draw_interface(game g, char* id);
void getHelp(int input, bool* done);
void input_player(game g, char* id);
bool checkFormat(char* s, char* format);

#endif