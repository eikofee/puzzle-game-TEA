#ifndef _INTERFACE_TXT_H_
#define	_INTERFACE_TXT_H_

void display_pieces(piece *p, int taille);
game test_level();
game getGameFromId(char*  id);
char* getIdFromGame(game g);
void draw_interface(game g, char* seed);
bool str_equal(char* a, char* b);
bool isNumber(char s, int max_number);
bool isOperatorSimple(char s);
int getNumber(char s);
dir getDirection(piece p, char sign);
void confirm();
void getHelp(int input, bool* done);
void input_player(game g);

#endif