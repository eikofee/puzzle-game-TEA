#ifndef _FUNCTION_INTERFACE_H_
#define _FUNCTION_INTERFACE_H_

void display_pieces(piece *p, int taille);
void toLower(char* s);
void setColorPiece(char c, int id, bool fill);
bool str_equal(char* a, char* b);
bool isNumber(char s, int max_number);
bool isOperatorSimple(char s);
int getNumber(char s);
dir getDirection(piece p, char sign);
void confirm();
void freeTableau2D(int** tab);
char getHexa(int n);
void ignoreOverflow(char* input, int taille);
void saveGameFromId(game g, char* id);
void loadGameFromSave(FILE* fichier, game g);
char* loadGameFromNum(FILE* fichier, char* num);

#endif