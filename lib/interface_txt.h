#ifndef _INTERFACE_TXT_H_
#define	_INTERFACE_TXT_H_

#include "function_interface.h"

/**
 * Permet d'afficher la jeu passé en paramètre en version Texte.
 * @param g Le jeu qui va être affiché.
 * @param id L ID correspondant au jeu qui va être affiché.
 */
void draw_interface(game g, char* id);

/**
 * Permet l'affichage du menu d'Aide.
 * @param input Un entier qui permet de savoir à quelle information on veut acceder
 * @param done Un booleen qui permet de savoir si on est toujours dans le menu Help ou si on le quitte
 */
void getHelp(int input, bool* done);

/**
 * Cette fonction gère les Inputs passés dans la console.
 * @param g Le jeu en cours qui va être modifié selon les inputs.
 * @param id L'ID correspondant au jeu en cours.
 */
void input_player(game g, char* id);

/**
 * Permet de vérifier si la 1ere Chaine de Caractere passée en paramètre suit un format cohérent
 * @param s Une chaine de caractère que l'on veut tester.
 * @param format Le format qu'on attendait de la chaine s.
 * @return Un booleen, True si la chaine correspond au format, False sinon.
 */
bool checkFormat(char* s, char* format);

#endif