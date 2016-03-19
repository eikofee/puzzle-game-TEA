#include <stdio.h>
#include <stdlib.h>
#include <game.h>
#include <utility.h>
#include <interface_txt.h>

void draw_interface(game g, char* id)
{
	int moves = game_nb_moves(g);
	piece* p = copieTableauPieces(g);
	int** t = mapPieces(p , game_nb_pieces(g), game_width(g), game_height(g));
	freeTableauDePieces(p, game_nb_pieces(g));
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

			case 3: //Sortie du parking (RH only)
				if (whatGame("rush-hour\n"))
					printf(">>\n");
				else
					printf("\x1b[47;90m##\x1b[0m\n");
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
	{
		if (i == game_width(g) / 2 - 1 && whatGame("klotski\n"))
			{
				printf("#|\x1b[0m vv \x1b[47;90m|#");
				i+=3;
			}else{
			printf("##");		//Affiche le bord inférieur
			}
	}
	printf("\x1b[0m\n");
	freeTableau2D(t);
	free(toWrite);
}


void getHelp(int input, bool* done)
{
	switch(input)
	{
		case 1:
			printf("\tRules are simple. You're the piece #0 and you need to go\n");
			printf("\tto the exit (\">>\"). To do that, you need to move the other\n");
			printf("\tpieces to free yourself a passage. Pieces can't cross\n");
			printf("\tothers or go outside the game area.\n");
			confirm();
			break;
		case 2:
			printf("\tSyntax is as it follows : \"[piece #] [direction] [distance]\"\n\n");
			printf("\t [piece #] : The number of the piece you want to move.\n");
			printf("\t [direction] : The direction you want to move the piece to.\n");
			printf("\t\tPossible directions:\n\t\t- u, U : UP\n\t\t- d, D : DOWN\n\t\t- l, L : LEFT\n\t\t- r, R : RIGHT\n");
			printf("\t [distance] : The number of cases you want to cross.\n\t               Negative numbers are allowed.\n\n");
			printf("\te.g.: '2 d 3' > move the piece #2 from 3 cases to the bottom.\n");
			confirm();
			break;
		case 3:
			printf("\tList of available commands:\n\n");
			printf("\thelp: Display this menu\n");
			printf("\t[WIP] hint: Get closer to the end by cheating\n");
			printf("\t[WIP] skip: Skip the current game for another\n");
			printf("\tid: Display the current game's ID\n");
			printf("\t[WIP] save: Save the current level for later\n");
			printf("\tload: Load a level\n");
			printf("\texit: Close the game\n");
			confirm();
			break;
		case 4:
			*(done) = true;
			break;
	}
}

/*
	Permet de fusionner plusieurs inputs
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

bool checkFormat(char* s, char* format)
{
	/*Syntaxe:
		%n : le char est un nombre
		%o : le char est un operateur simple
		%i : int
		%d : direction (u, d, l, r)
		%e : un nombre indéfini d'espaces
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

				case 'e':
					while(s[j] == ' ')
						j++;
					j--;
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
	removeSpaces(input);
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
		//saveGameFromId(g, id);
		printf("save [WIP]");
	}
	if (str_equal(input, "id\n"))
	{
		correct = true;
		printf("Game ID : %s\n", id);
	}
	if (str_equal(input, "load\n"))
	{
		correct = true;
		//On charge la partie que l'on veut, save 1 2 ou 3
		//On alloue le char* level qui va prendre l'input du fgets
		
		printf("level number :\n(1, 2 or 3 or type 'c' to cancel)\n");
		char* level = (char*)malloc(sizeof(char)*256);
		fgets(level, 256, stdin);

		if (str_equal(level, "c"))
			correct = false;

		// Ce bloc est mis en commentaire uniquement pour la V2 . . . 
		// if(str_equal(level, "save\n"))
		// {
		// 	printf("[WIP]\n");
		// 	//loadGameFromSave("save.txt", g);
		// 	//free(level);
		// 	//return;
		// }

		if (correct)
		{
			//On met dans new_id le nouvel id du niveau chargé
			//On recopie le new_id dans id et on libère level et new_id
			
			char* new_id;
			if(whatGame("rush-hour\n"))
				new_id = loadGameFromNum("games_rh.txt", level);
			else if(whatGame("klotski\n"))
				new_id = loadGameFromNum("games_ar.txt", level);
			sprintf(id,"%s",new_id);
			free(new_id);
			game g2 = getGameFromId(id);
			copy_game(g2, g);
			delete_game(g2);
		}
		free(level);
		correct = true;
		return;
	}
	
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

		if (checkFormat(input, "%i\n"))
		{
			//input multiple
			fuseNewInput(input, "%d", &pos, "Enter a direction:");
			fuseNewInput(input, "%i", &pos, "Enter a distance:");
			correct = true;
		}
		if (checkFormat(input, "%i%e%d%e%i"))
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
		}
	}
	getIdFromGame(g, id);
}