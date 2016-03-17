#ifndef _FUNCTION_INTERFACE_H_
#define _FUNCTION_INTERFACE_H_

void display_pieces(piece *p, int taille);
void toLower(char* s);
void setColorPiece(char c, int id, bool fill);
bool str_equal(char* a, char* b);
bool isNumber(char s, int max_number);
bool isInt(char* s, int* pos);
bool isDirection(char s);
bool isOperatorSimple(char s);
int getNumber(char s);
//dir getDirection(piece p, char sign);
void confirm();
void freeTableau2D(int** tab);
char getHexa(int n);
void ignoreOverflow(char* input, int taille);
void saveGameFromId(game g, char* id);
void loadGameFromSave(char* fichier, game g);
char* loadGameFromNum(char* fichier, char* num);
dir getDirection(char* s, int* pos);
void revertDirection(dir* d);

piece* copieTableauPieces(game g);
void freeTableauDePieces(piece* pieces, int nb_pieces);
game getGameFromId(char*  id);
void getIdFromGame(game g, char* id);
piece getPieceFromId(char* id, int* pos);
int readUntilChar(char* s, int* pos);
void getCharFromInt(char* s, int* pos, int data);
void initFileConfig(char* level_name);
#endif