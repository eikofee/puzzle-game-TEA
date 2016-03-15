#include <stdio.h>
#include <stdlib.h>
#include <game.h>
#include <utility.h>
#include <interface_txt.h>

// TODO : 	[Done] Gerer differement le déplacement d'une piece.
//			[Done] Revoir la construction d'une id
// 			[Done] Fonctions utiles exclusivement à RUsh hour : getDirection()



//Ce fichier permet de gérer l'affichage en mode texte du jeu


bool checkFormat(char* s, char* format);
int readUntilChar(char* s, int* pos)
{
	int n = 0;
	int multiplier = 1;
	while (s[*pos] == '+' || s[*pos] == '-')
	{
		if (s[*pos] == '-')
			multiplier *= -1;
		*pos += 1;
	}
	while (s[*(pos)] >= '0' && s[*(pos)] <= '9')
	{
		n *= 10;
		n += s[*(pos)] - '0';
		*(pos) += 1;
	}

	return n * multiplier;
}

void getCharFromInt(char* s, int* pos, int data)
{
	if (!data)
	{
		s[*(pos)] = '0';
		*(pos) += 1;
		return;
	}
	int taille = 1;
	int data_calc = data / 10;
	while (data_calc > 0)
	{
		data_calc /= 10;
		taille *= 10;
	}
	while (taille > 0)
	{
		s[*pos] = data / taille + '0';
		*(pos) += 1;
		data -= data / taille * taille;
		taille /= 10;
	}
}

/*Syntaxe version 2:
		(nb_pieces)n(taille_x)x(taille_y)p(1,2 ou 3)w(width)h(height)x(pos_x)y(pos_y)p(next)
		can_move_x = +1
		can_move_y = +2
	*/
void getIdFromGameAR(game g, char* id)
{
	int pos = 0;
	int n_piece = 0;
	getCharFromInt(id, &pos, game_nb_pieces(g));
	id[pos] = 'n';
	pos++;
	getCharFromInt(id, &pos, g -> width);
	id[pos] = 'x';
	pos++;
	getCharFromInt(id, &pos, g -> height);
	while (n_piece < game_nb_pieces(g))
	{
		id[pos] = 'p';
		pos++;
		id[pos] = '0' + (can_move_x(game_piece(g, n_piece))?1:0) + (can_move_y(game_piece(g, n_piece))?2:0);
		pos++;
		id[pos] = 'w';
		pos++;
		getCharFromInt(id, &pos, get_width(game_piece(g, n_piece)));
		id[pos] = 'h';
		pos++;
		getCharFromInt(id, &pos, get_height(game_piece(g, n_piece)));
		id[pos] = 'x';
		pos++;
		getCharFromInt(id, &pos, get_x(game_piece(g, n_piece)));
		id[pos] = 'y';
		pos++;
		getCharFromInt(id, &pos, get_y(game_piece(g, n_piece)));
		n_piece++;
	}
	id[pos] = '\0';
}

piece getPieceFromIdAR(char* id, int* pos)
{
	int w = 0;
	int h = 0;
	int x = 0;
	int y = 0;
	int type = 0;
	int state = 0; // 0 = type, 1 = w, 2 = h, 3 = x, 4 = y
	while (id[*(pos)] != 'p' && id[*(pos)] != '\0')
	{
		switch(state)
		{
			case 0:
				type = readUntilChar(id, pos);
				break;
			case 1:
				w = readUntilChar(id, pos);
				break;
			case 2:
				h = readUntilChar(id, pos);
				break;
			case 3:
				x = readUntilChar(id, pos);
				break;
			case 4:
				y = readUntilChar(id, pos);
				*(pos) -= 1;
				break;
		}
		state++;
		if (id[*(pos)])
			*(pos) += 1;
	}
	piece p = new_piece(x, y, w, h, ((type >= 2)?true:false), ((type == 1 || type == 3)?true:false));
	return p;
	
}

/*Syntaxe version 2:
		(nb_pieces)n(taille_x)x(taille_y)p(1,2 ou 3)w(width)h(height)x(pos_x)y(pos_y)p(next)
	*/
game getGameFromIdAR(char* id)
{
	int i = 0;
	int nb_pieces = readUntilChar(id, &i);
	i++;
	piece p[nb_pieces];
	int n_piece = 0;	//index de p
	int taille_x = 0;
	int taille_y = 0;
	int state = 1; // 1 = taille_x, 2 = taille_y, 3 = piece
	piece p_tmp;
	

	while (id[i] != '\0')
	{
		switch(state)
		{
			case 1:
				taille_x = readUntilChar(id, &i);
				break;
			case 2:
				taille_y = readUntilChar(id, &i);
				break;
			case 3:

				p_tmp = getPieceFromIdAR(id, &i);
				p[n_piece] = new_piece(0, 0, 0, 0, true, true);
				copy_piece(p_tmp, p[n_piece]);
				n_piece++;
				state--;
				delete_piece(p_tmp);
				break;
		}
		if (id[i])
			i++;
		state++;
	}
	game g = new_game(taille_x, taille_y, nb_pieces, p);
	return g;
}
/*
	Permet de générer un niveau à partir d'un id (non seed)
*/
game getGameFromIdRH(char* id)
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
		p[indexP] = new_piece_rh(x, y, true, isHorizontal); //isSmall
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
/*void getIdFromGame(game g, char* id)
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
*/

/*
	Affiche la zone de jeu
*/
	// ************************* Changement de la forme TableauDePieces(tableau de piece, taille axe x, taille axe y)
void draw_interface(game g, char* id)
{
	int moves = game_nb_moves(g);
	int** t = TableauDePieces(g -> pieces, game_nb_pieces(g), game_width(g), game_height(g));
	bool* toWrite = (bool*) malloc(sizeof(bool) * game_nb_pieces(g));
	for (int i = 0; i < game_nb_pieces(g); i++)
		toWrite[i] = true;

	printf("\x1b[47;90m##");
	for (int i = 0; i < game_width(g); i++)
		printf("##");			//Affiche le bord supérieur
	printf("##\x1b[0m Rush Hour or Red Klotski VERSION 2\n");
	for (int i = game_height(g) - 1; i > -1; i--)
	{
		printf("\x1b[47;90m##\x1b[0m");
		for (int j = 0; j < game_width(g); j++)
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
	printf("\x1b[47;90m");
	for (int i = 0; i < game_width(g) + 2; i++)
		printf("##");		//Affiche le bord inférieur
	printf("\x1b[0m\n");
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
			printf("\tSyntax is as it follows : \"a b\" where a is the car number you want to move and b the distance to\n");
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
	Permet de fucionner plusieurs inputs
*/
void fuseNewInput(char* input, char* expectedFormat, int* pos, char* information)
{
	char newInput[] = "...............";
	while (!checkFormat(newInput, expectedFormat))
	{
		
		printf("%s\n", information);
		fgets(newInput, 15, stdin);
	}
	while (input[*pos] != ' ' && input[*pos] != '\n')
	{
		*pos += 1;
	}
	int i = 0;
	while(newInput[i] != '\n')
	{
		input[*pos] = newInput[i];
		*pos += 1;
		i++;
	}
	input[*pos] = '\n';
}
/*
	Verifie si s suit un format correct (un peu a la facon de printf)
*/
bool checkFormat(char* s, char* format)
{
	/*Syntaxe:
		%n : le char est un nombre
		%o : le char est un operateur simple
		%i : int
		%d : direction (u, d, l, r)
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

				case 'i':
					if (!isInt(s, &j))
						return false;
					break;

				case 'd':
					if (!isDirection(s[j]))
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
	char input[64];
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
		game g2 = NULL; //getGameFromId(id);
		copy_game(g2, g);
		delete_game(g2);
		return;
	}
	
	//if (isNumber(input[0], g -> nb_pieces - 1))
	if (checkFormat(input, "%i"))
	{
		int n_piece = 0;
		dir direction = UP;
		int distance = 0;
		int pos = 0;
		/*SYNTAXE :
			0 2 : Avance la voiture rouge de 2 cases vers la droite
			1 -1 : Recule la voiture 1 vers le bas si verticale ou la gauche si horizontale
			2 : Demande un dÃ©placement de la voiture 2 (nouveau scanf)
		*/

		//OLD 
		/*if (input[1] == '\n')
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
		}*/

		//NEW
		if (checkFormat(input, "%i\n"))
		{
			//input multiple
			fuseNewInput(input, "%d", &pos, "Enter a direction:");
			fuseNewInput(input, "%i", &pos, "Enter a distance:");
			correct = true;
		}
		if (checkFormat(input, "%i %d %i") || checkFormat(input, "%i%d%i"))
		{
			pos = 0;
			n_piece = readUntilChar(input, &pos);
			if (checkFormat(input, "%i %d %i"))
				pos++;
			direction = getDirection(input, &pos);
			pos++;
			if (checkFormat(input, "%i %d %i"))
				pos++;
			distance = readUntilChar(input, &pos);

			//ignoreOverflow(input, pos + 1);
			correct = true;
		}
		if (distance != abs(distance))
			revertDirection(&direction);
		play_move(g, n_piece, direction, abs(distance));
	}
	else
	{
		if (!correct)
		{
			printf("Incorrect input. Type 'help' for more informations.\n");
			//ignoreOverflow(input, 6);
		}
	}
	//getIdFromGame(g, id);
}