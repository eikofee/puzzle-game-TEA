#ifndef _FUNCTION_INTERFACE_H_
#define _FUNCTION_INTERFACE_H_

/**
 * Permet d'afficher les informations des pieces du tableau
 * Cette fonction est utilisé pour du DEBUG dans les fonctions de test sur
 * game.c et piece.c
 * @param array_pieces Tableau de pieces
 * @param size Le nombre de piece à afficher
*/
void displayPieces(piece *array_pieces, int size);

/**
 * Change les majuscules en minuscule d'une chaine de caractère
 * @param s La chaine de Caractere que l'on veut modifier
 */
void toLower(char* s);

/**
 * Change la couleur d'une pièce en fonction de son numéro
 * @param c  Le caractère à afficher
 * @param id Le numéro de la pièce à afficher
 * @param fill Est-ce que le caractère doit être imprimé ?
 */
void setColorPiece(char c, int id, bool fill);

/**
 * Test deux chaines de caractere pour savoir si elles sont identiques.
 * @param a Une chaine de caractere
 * @param b Une autre chaine de Caractere
 * @return Un booleen, True si elles sont identiques, False sinon.
 */
bool strEqual(char* a, char* b);

/**
 * Verifie si le char passé en paramètre est un chiffre et ne dépasse pas max_number
 * @param c Le caractère que l'on veut tester
 * @param max_number La valeur maximum que c ne doit pas dépasser
 * @return Un booleen, True si le caractere est un Chiffre, False sinon.
 */
bool isNumber(char c, int max_number);

/**
 * Verifie que la chaine de caractère s commence bien par un nombre
 * @param s La chaine de caractère
 * @param pos Le curseur parcourant la chaine (permet d'enchainer les vérifications)
 * @return Un booleen, True si la chaine s commence par un nombre, False sinon.
 */
bool isInt(char* s, int* pos);

/**
 * Verifie que le caractère correspond à une direction (u, d, l, r)
 * @param c Le caratere que l'on test.
 * @return Un booleen, True si `c` est bien une direction, False sinon
 */
bool isDirection(char s);

/**
 * Verifie si le caractere `c` est un + ou un -
 * @param c Un caractere
 * @return Un booleen, True si c est un + ou un -, False sinon.
 */
bool isOperatorSimple(char c);

/**
 * Convertit un caractere en un int (hexa minuscule permis)
 * @param c Un caractere
 * @return Un entier correspondant au caractère.
 */
int getNumber(char c);
//dir getDirection(piece p, char sign);

/**
 * Bloque l'affichage tant que la touche Entrée n'est pas utilisée
 */
void confirm();

/**
 * Permet la libération d'un tableau 2D d'entiers
 * tab Un tableau 2D d'entier.
 */
void freeTableau2D(int** tab);

/**
 * Convertit un int en char (hexa minuscule)
 * @param n Un entier
 * @return Le caractère correspondant à n.
 */
char getHexa(int n);

// void saveGameFromId(game g, char* id);
// void loadGameFromSave(char* fichier, game g);

/**
 * Permet de charger un Niveau
 * @param file Une chaine de caractere correspondant au Fichier que l'on souhaite ouvrir
 * @param num Le numéro du level que l'on souhaite ouvrir
 * @return L'ID du niveau en question.
 */
char* loadGameFromNum(char* file, char* num);

/**
 * Convertir la direction de l'input en enumeration `dir`
 * @param s Une chaine de caractere
 * @param pos Le curseur parcourant la chaine (permet d'enchainer les vérifications)
 * @return La direction correspondante à l'input.
 */
dir getDirection(char* s, int* pos);

/**
 * Permet d'obtenir la direction inverse à celle passée en paramètre
 * en la modifiant directement.
 * @param d Une Direction `dir`
 */
void revertDirection(dir* d);

/**
 * Crée une copie du tableau de piece du jeu `g`
 * @param g Un jeu game.
 * @return Une copie du tableau de piece de `g`
 */
piece* copyTableauPieces(game g);

/**
 * Permet de liberer un tableau de pièce (free)
 * @param pieces Un tableau de pieces
 * @param nb_pieces Le nombre de piece du tableau.
 */
void freeTableauDePieces(piece* pieces, int nb_pieces);

/**
 * Permet d'obtenir un jeu à partir d'un ID
 * @param id Une chaine de caractere correspondant à un jeu
 * @return Un jeu `game` avec les informations de l'ID
 */
game getGameFromId(char*  id);

/**
 * Permet d'obtenir l'ID d'un jeu
 * @param g Un jeu `game`
 * @param id Une chaine de caractere qui va recevoir l'ID correspondant à `g`
 */
void getIdFromGame(game g, char* id);

/**
 * Permet d'obtenir une piece a partir d'une position dans l'ID
 * @param id Une chaine de caractere correspondant a l ID
 * @param pos La position actuelle dans l'ID
 * @return Une piece
 */
piece getPieceFromId(char* id, int* pos);

/**
 * Permet d'obtenir un nombre dans une chaine de caractère jusqu'a ce qu'on rencotre un caractere
 * @param s Une chaine de caractère
 * @param pos Un pointeur sur la position dans la chaine s
 * @return l'entier lu jusqu'a un caractère
 */
int readUntilChar(char* s, int* pos);

/**
 * Permet de recuperer une chaine de caractère à partir d'un int
 * @param s Une chaine de caractere, dans laquelle on va ecrire l'informatin int
 * @param pos La position actuelle dans la chaine de caractere `s`
 * @param data L'information que l'on veut écrire dans la chaine `s`
 */
void getCharFromInt(char* s, int* pos, int data);

/**
 * Permet d'écrire dans le fichier config.ini le nom du jeu en cours d'execution
 * @param level_name Une chaine de caractere correspondant au nom du jeu en cours
 */
void initFileConfig(char* level_name);

/**
 * Permet de récuperer l'information stockée dans config.ini pour savoir quel jeu est en cours d'execution
 * @param name Une chaine de caractère, va être utilisé pour savoir si name est égal au nom du jeu stocké dans config.ini
 * @return Un booleen, True si name est égal au nom dans config.ini, false sinon.
 */
bool whatGame(char* name);

/**
 * Permet de retirer les espaces dans une chaine de caractère
 * @param input La chaine de caractère qui va être modifié.
 */
void removeSpaces(char* input);
#endif