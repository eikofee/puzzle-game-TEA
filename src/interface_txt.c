#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "utility.h"
#include "interface_txt.h"

// TODO : 	Gerer differement le déplacement d'une piece.
//			Revoir la construction d'une id
// 			Fonctions utiles exclusivement à RUsh hour : getDirection()



//Ce fichier permet de gérer l'affichage en mode texte du jeu

/*
	Permet de générer un niveau à partir d'un id (non seed)
*/
game getGameFromId(char* id)
{
	//Syntaxe : Nabca2b2c2a3b3c3
	//N = nombre de voitures
	//a = car type: +1 st horizontale, +2 si grand
	//b = case axe x
	//c = case axe y
	int nb_pieces = getNumber(id[0]) ;
	piece p[nb_pieces];
	int i = 1;
	int indexP = 0;

	while (id[i] != '\0')
	{
		bool isHorizontal = (id[i] == '1' || id[i] == '3');
		bool isSmall = (id[i] == '0' || id[i] == '1');
		int x = getNumber(id[i + 1]);
		int y = getNumber(id[i + 2]);
		p[indexP] = new_piece_rh(x, y, isSmall, isHorizontal);
		i += 3;
		indexP++;
	}
	game g = new_game_hr(nb_pieces, p);

	for(int j = 0; j < indexP; j++)
		free(p[j]);

	return g;
}

/*
	Permet de convertir un game en id (pas une sauvegarde complète)
*/
void getIdFromGame(game g, char* id)
{
	id[0] = getHexa(game_nb_pieces(g));
	int indexChar = 1;
	for (int i = 0; i < game_nb_pieces(g); i++)
	{
		id[indexChar] = getHexa((is_horizontal(g -> pieces[i])?1:0) + ((g -> pieces[i] -> isSmall)?0:2));
		id[indexChar + 1] = getHexa(get_x(g -> pieces[i]));
		id[indexChar + 2] = getHexa(get_y(g -> pieces[i]));
		indexChar += 3;
	}
	id[indexChar] = '\0';
}


/*
	Affiche la zone de jeu
*/
void draw_interface(game g, char* id)
{
	int moves = game_nb_moves(g);
	int** t = TableauDePieces(g -> pieces, game_nb_pieces(g));
	bool* toWrite = (bool*) malloc(sizeof(bool) * game_nb_pieces(g));
	for (int i = 0; i < game_nb_pieces(g); i++)
		toWrite[i] = true;

	printf("\x1b[47;90m################\x1b[0m Rush Hour\n");
	for (int i = 5; i > -1; i--)
	{
		printf("\x1b[47;90m##\x1b[0m");
		for (int j = 0; j < 6; j++)
		{
			if (t[j][i] == -1)
				printf(". ");
			else
			{
				setColorPiece(getHexa(t[j][i]), t[j][i], (toWrite[t[j][i]]?true:false));
				toWrite[t[j][i]] = false;
			}
		}
		
		switch(i){
			//let's check if we have to display more informations on the right:
			case 0: 
				printf("\x1b[47;90m##\x1b[0m Type 'help' for more informations\n");
				break;
			case 1: //ID/Seed du jeu
				printf("\x1b[47;90m##\x1b[0m ID: %s\n", id);
				break;
			case 3: //Sortie du parking
				printf(">>\n");
				break;
			case 4:	//Move num display
				printf("\x1b[47;90m##\x1b[0m Move %d\n", moves);
				break;
			default:
				printf("\x1b[47;90m##\x1b[0m\n");
				break;
		}
	}
	printf("\x1b[47;90m################\x1b[0m\n");
	freeTableau2D(t);
	free(toWrite);
}



/*
	Affiche l'interface d'aide du jeu
*/
void getHelp(int input, bool* done)
{
	switch(input)
	{
		case 1:
			printf("\tRules are simple. You're the car nÂ°0 and you need to go to the exit (\">\"). To do that, you need\n");
			printf("to move the other cars to free yourself a passage. Cars can't cross others or go outside the game area.\n");
			confirm();
			break;
		case 2:
			printf("\tSyntaxe is as it follows : a b where a is the car number you want to move and b the distance to\n");
			printf("translate it. Positive numbers go to up and right and negative ones go to down and left.\n");
			printf("e.g. 2 -3 with the car number 2 being a horizontal car will go 3 cases to the left. You can also input\n");
			printf("the car number first and then input the distance.\n");
			confirm();
			break;
		case 3:
			printf("\tList of available commands:\n\n");
			printf("\thelp: Display this menu\n");
			printf("\t[WIP]hint: Get closer to the end by cheating\n");
			printf("\t[WIP]skip: skip the current game for another\n");
			printf("\tsave: Save the current run for later\n");
			printf("\tload: Load a run\n");
			printf("\texit: Close the game\n");
			confirm();
			break;
		case 4:
			*(done) = true;
			break;
	}
}

/*
	Permet d'obtenir le mouvement de la piece entree dans un premier temps
	(Ameliore la lisibilite)
*/
void getSecondInput(char* input)
{
												//Current input is "[N][\n][ ][ ][ ][ ]"
	char input2[5];
	printf("Enter the distance for car n°%c :\n", input[0]);
	fgets(input2, 4, stdin);
	input[1] = ' ';								//Current input is "[N][_][ ][ ][ ][ ]"					
	if (isOperatorSimple(input2[0]))
	{
		input[2] = input2[0];					//Current input is "[N][_][+-][ ][ ][ ]"
		input[3] = input2[1];					//Current input is "[N][_][+-][n][ ][ ]"
		input[4] = '\n';
	}else{
		input[2] = input2[0];
		input[3] = '\n';					//Current input is "[N][_][n][ ][ ][ ]"
	}
	ignoreOverflow(input2, 4);
}

/*
	Verifie si s suit un format correct (un peu a la facon de printf)
*/
bool checkFormat(char* s, char* format)
{
	/*Syntaxe:
		%n : le char est un nombre
		%o : le char est un operateur simple
	*/
	int i = 0;
	int j = 0;
	while (format[i] != '\0')
	{
		if (format[i] != '%')
		{
			if (s[j] != format[i])
				return false;
		}
		else{
			switch(format[++i])
			{
				case 'n':
					if (!isNumber(s[j], 9))
						return false;
					break;

				case 'o':
					if (!isOperatorSimple(s[j]))
						return false;
					break;
				default:
					error("Syntaxe checkFormat incorrecte.");
					break;
			}
		}
		i++;
		j++;
	}
	return true;
}


/*
	Recupere les commandes du joueur (imparfait)
	Permet de gerer les inputs pour deplacer les vehicules, ainsi que de gerer les menus
*/
void input_player(game g, char* id)
{
	char input[7] = {0, 0, 0, 0, 0, 0, 0};
	fgets(input, sizeof(input), stdin);
	toLower(input);
	bool correct = false;
	if (str_equal(input, "help\n"))
	{
		correct = true;
		bool done = false;
		while (!done)
		{
			char i[4];
			printf("What do you want to know ? (Type a number)\n\n\t1. What's this game ?\n\t2. How to play\n\t3. Advanced commands\n\t4. Get back to the game\n");
			fgets(i, 3, stdin);
			getHelp(getNumber(i[0]), &done);
		}
	}

	if (str_equal(input, "hint\n"))
	{
		correct = true;
		printf("hint [WIP]\n");
		//Do a move 
	}
	if(str_equal(input, "skip\n"))
	{
		correct = true;
		printf("skip [WIP]\n");
	}
	if (str_equal(input, "exit\n"))
	{
		correct = true;
		//exit the game
		free(id);
		delete_game(g);
		exit(EXIT_SUCCESS);
	}
	if (str_equal(input, "save\n"))
	{
		correct = true;
		//sauvegarde
		saveGameFromId(g, id);
	}
	if (str_equal(input, "load\n"))
	{
		correct = true;
		//On charge la partie que l'on veut, save 1 2 ou 3
		//On alloue le char* level qui va prendre l'input du fgets
		
		printf("level number :\n(1, 2 or 3 | Type save to load your last save)\n");
		char* level = (char*)malloc(sizeof(char)*128);
		fgets(level, 128, stdin);

		if(str_equal(level, "save\n"))
		{
			loadGameFromSave("save.txt", g);
			free(level);
			return;
		}
		//On met dans new_id le nouvel id du niveau chargé
		//On recopie le new_id dans id et on libère level et new_id
		
		char* new_id;
		new_id = loadGameFromNum("games.txt", level);
		sprintf(id,"%s",new_id);
		free(level);
		free(new_id);
		game g2 = getGameFromId(id);
		copy_game(g2, g);
		delete_game(g2);
		return;
		
	}
	
	if (isNumber(input[0], g -> nb_pieces - 1))
	{
		correct = true;
		/*SYNTAXE :
			0 2 : Avance la voiture rouge de 2 cases vers la droite
			1 -1 : Recule la voiture 1 vers le bas si verticale ou la gauche si horizontale
			2 : Demande un dÃ©placement de la voiture 2 (nouveau scanf)
		*/ 
		if (input[1] == '\n')
			getSecondInput(input);
		if (checkFormat(input, "%n %n")) // ********** FAIRE LES MODIFICATIONS SUR getDirection ICI ********
		{
			play_move(g, getNumber(input[0]), getDirection(g -> pieces[getNumber(input[0])], '+'), getNumber(input[2]));
		}
		if (checkFormat(input, "%n %o%n"))
		{
			if (input[2] == '+')
				play_move(g, getNumber(input[0]), getDirection(g -> pieces[getNumber(input[0])], '+'), getNumber(input[3]));
			else{
				play_move(g, getNumber(input[0]), getDirection(g -> pieces[getNumber(input[0])], '-'), -1 * getNumber(input[3]));
			}
		}
	}
	else
	{
		if (!correct)
		{
			printf("Incorrect input. Type 'help' for more informations.\n");
			ignoreOverflow(input, 6);
		}
	}
	getIdFromGame(g, id);
}

