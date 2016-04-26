#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

#include <interface_sdl.h>
#include <utility.h>



//Cette structure permet la création de bouton. Nous définissons un bouton comme un rectangle, par son coin supérieur gauche, sa largeur/hauteur.
typedef struct button_s* button;
struct button_s{
	int x;
	int y;
	int width;
	int height;
};

//Crée et retourne un boutton
button createButton(int x, int y, int width, int height){
	button newButton = (button)malloc(sizeof(struct button_s));
	if(newButton == NULL)
		error("createButton(), probleme d'allocation du bouton");
	newButton->x = x;
	newButton->y = y;
	newButton->width = width;
	newButton->height = height;
	return newButton;
}

//Vérifie si les coordonnées (xMouse;yMouse) sont bien dans la surface du boutton "Button"
bool checkButton(int xMouse, int yMouse, button Button){
	if(Button == NULL)
		error("checkButton(), le bouton n'est pas alloué ou n'existe pas ...");
	return ( (xMouse >= (Button->x)) && (xMouse < ((Button->x) + (Button->width))) && (yMouse >= (Button->y)) && (yMouse < ((Button->y) + (Button->height))));
}

//Vérifie si les coordonnées (xMouse;yMouse) sont bien dans la surface du boutton "Button"
void hoverButton(SDL_Surface *screen, char* str, button Button, SDL_Color colorWrite, SDL_Color colorBackground, TTF_Font *font){
	SDL_Rect position;
	position.x = Button->x;
	position.y = Button->y;

	SDL_Surface *text = NULL;
	text = TTF_RenderText_Shaded(font, str, colorWrite, colorBackground);

	SDL_BlitSurface(text, NULL, screen, &position);
}

//Effectue un free sur un Button non null.
void deleteButton(button Button){
	if(Button == NULL)
		error("deleteButton(), Button n'est pas alloué ou n'existe pas ...");
	free(Button);
}

//Retourne le nom du jeu sous forme de char* (config.ini)
char* whatGameStr()
{
	FILE *file = NULL;
	file = fopen("config.ini", "r");
	if(file == NULL)
		error("whatGameStr(), probleme d'ouverture du fichier config.ini");
	
	char* str = (char*)malloc(sizeof(char) * 256);
	fgets(str, 256, file);
	fclose(file);

	int i = 0;
	while(str[i] != '\0')
	{
		if(str[i] == '\n')
		{
			str[i] = '\0';
			return str;
		}
		i++;
	}
	return str;  
}

int min(int a, int b)
{
	if (a > b)
		return b;
	return a;
}

//Affiche a l'ecran un jeu "game g"
void displayGridGame(game g, SDL_Surface *screen, SDL_Surface ***grid, int NL, int NH, int SIZE_SQUARE, int selectedPiece){
	SDL_Rect position;
	//Ce tableau contient les couleurs que peuvent avoir une pièce. (r,g,b)
	int nb_Colors = 11;
	int array_colors[11][3] = {{231, 76, 60}, {255, 51, 153}, {155, 89, 182}, {52, 152, 219}, {46, 204, 113}, {52, 73, 94}, {241, 196, 15}, {230, 126, 34}, {246, 36, 89}, {51, 110, 123}, {31, 58, 147}};
	int decalZero = (!selectedPiece?0:1);
	float lighterRatio = 1.3;
	int colorLighter[3] = {min(array_colors[selectedPiece % nb_Colors + decalZero][0]*lighterRatio, 255), min(array_colors[selectedPiece % nb_Colors + decalZero][1] *lighterRatio, 255), min(array_colors[selectedPiece % nb_Colors + decalZero][2] *lighterRatio, 255)};
	//On parcours chaque rectangle, et pour chaque re)ctangle, on regarde son equivalent dans le jeu. Si le jeu renvoi -1 alors y'a rien, donc gris.
	//Sinon, le jeu va renvoyer l'indice associé au rectangle, et ainsi on va lui donner une couleur via le tableau.
	//La couleur d'indice 0 est réservée exclusivement pour la voiture principale du même indice, elle est la seule rouge.
	int i = 0;
	for(int y = 0; y < NH; y++)
	{
		for(int x = 0; x < NL; x++)
		{
			int indexPiece = game_square_piece(g, x, abs(y-NH +1));
			
			position.x = x*SIZE_SQUARE + i;
			position.y = y*SIZE_SQUARE + i;
			if(indexPiece != -1)
			{
				if(indexPiece == 0)
				{
					if (indexPiece != selectedPiece)
						SDL_FillRect(grid[y][x], NULL, SDL_MapRGB(screen->format, array_colors[0][0], array_colors[0][1], array_colors[0][2]));
					else
						SDL_FillRect(grid[y][x], NULL, SDL_MapRGB(screen->format, colorLighter[0], colorLighter[1], colorLighter[2]));
				}
				else{
					if (indexPiece != selectedPiece)
						SDL_FillRect(grid[y][x], NULL, SDL_MapRGB(screen->format, array_colors[indexPiece%(nb_Colors-1) + 1][0], array_colors[indexPiece%(nb_Colors-1) + 1][1], array_colors[indexPiece%(nb_Colors-1) + 1][2]));
					else
						SDL_FillRect(grid[y][x], NULL, SDL_MapRGB(screen->format, colorLighter[0], colorLighter[1], colorLighter[2]));
				}
			}
			else
				SDL_FillRect(grid[y][x], NULL, SDL_MapRGB(screen->format, 210, 210, 210));

			SDL_BlitSurface(grid[y][x], NULL, screen, &position);
		}
	}
}

//Retourne l'indice de la pièce cliquée par la souris. -1 si le clic ne s'est pas effectué sur une pièce.
int clic(SDL_Event event, game g, int WIDTH, int HEIGHT, int NL, int NH, int SIZE_SQUARE){
	//On recupere les coordonnées du clic
	int xMouse = event.button.x;
	int yMouse = event.button.y;

	//On convertit ces coordonnées en coordonnées plateau
	xMouse = xMouse / SIZE_SQUARE;
	yMouse = yMouse / SIZE_SQUARE;

	//on re oriente l'axe des ordonnées pour correspondre à l'orientation du jeu.
	yMouse = abs(yMouse-NH +1);

	//On regarde si le clic s'est bien produit sur le plateau
	if(xMouse < 0 || xMouse >= NL || yMouse < 0 || yMouse >= NH)
		return -1;
	//ici les coordonnées sont valides donc on peut les manipuler.

	int indexPiece = game_square_piece(g, xMouse, yMouse);
	if(indexPiece < 0)
		return -1;
	return indexPiece;
}

//Affiche un petit menu echap lorsqu'on appuie sur la touche echap.
void menu_echap(SDL_Surface *screen, int *MAIN_CHECK, int *CHECK, int WIDTH, int HEIGHT, SDL_Color color1, SDL_Color color2, TTF_Font *font){
	//on crée une surface screenTMP pour conserver l'ancien écran, afin de le réafficher à la fin de la fonction.
	SDL_Surface *screenTMP = SDL_CreateRGBSurface(SDL_HWSURFACE, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
	SDL_Surface *text = NULL;
	SDL_Surface *menuEchap = NULL;

	SDL_Color newColorHover = {149, 165, 166};

	SDL_Event event;
	SDL_Rect position;
	position.x = 0;
	position.y = 0;
	SDL_BlitSurface(screen, NULL, screenTMP, &position);

	//A partir d'ici, on va créer les deux bouttons Oui et Non, ainsi qu'afficher du text.
	text = TTF_RenderText_Blended(font, "Do you want to leave the game ?", color1);
	
	int widthEchap = text->w + 100;
	int heightEchap = HEIGHT/3;

	menuEchap = SDL_CreateRGBSurface(SDL_HWSURFACE, widthEchap,  heightEchap, 32, 0, 0, 0, 0);

	position.x = (WIDTH - (widthEchap))/2;
	position.y = (HEIGHT - (heightEchap))/2;
	SDL_FillRect(menuEchap, NULL, SDL_MapRGB(screen->format, 77, 83, 84)); //couleur Basalt
	SDL_BlitSurface(menuEchap, NULL, screen, &position);

	position.x = (position.x + ((position.x + widthEchap) - text->w))/2;
	position.y = (position.y + text->h);
	SDL_BlitSurface(text, NULL, screen, &position);

	SDL_FreeSurface(text);
	text = TTF_RenderText_Shaded(font, "   Yes   ", color2, color1);
	position.y = position.y - 3*text->h + heightEchap;

	button button_Oui = createButton(position.x, position.y, text->w, text->h);

	SDL_BlitSurface(text, NULL, screen, &position);

	SDL_FreeSurface(text);
	text = TTF_RenderText_Shaded(font, "   No   ", color2, color1);
	position.x += (widthEchap - 100) - text->w;

	button button_Non = createButton(position.x, position.y, text->w, text->h);

	SDL_BlitSurface(text, NULL, screen, &position);

	//On commence la boucle while, avec la variable ci dessous comme condition.
	int echapCheck = 1;
	int xMouse;
	int yMouse;

	while(echapCheck)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				echapCheck = 0;
				*CHECK = 0;
				*MAIN_CHECK = 0;
				break;
			case SDL_MOUSEMOTION:
				xMouse = event.motion.x;
				yMouse = event.motion.y;
				if(checkButton(xMouse, yMouse, button_Non))
					hoverButton(screen, "   No   ", button_Non, color1, newColorHover, font);
				else
					hoverButton(screen, "   No   ", button_Non, color2, color1, font);

				if(checkButton(xMouse, yMouse, button_Oui))
					hoverButton(screen, "   Yes   ", button_Oui, color1, newColorHover, font);
				else
					hoverButton(screen, "   Yes   ", button_Oui, color2, color1, font);
			break;


			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					xMouse = event.button.x;
					yMouse = event.button.y;
					if(checkButton(xMouse, yMouse, button_Oui))
					{
						echapCheck = 0;
						*CHECK = 0;
						*MAIN_CHECK = 0;
					}
					if(checkButton(xMouse, yMouse, button_Non))
						echapCheck = 0;
				}
				break;

			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						echapCheck = 0;
						break;
					case SDLK_y:
						*CHECK = 0;
						echapCheck = 0;
						*MAIN_CHECK = 0;
						break;
					case SDLK_n:
						echapCheck = 0;
					default:
						break;
				}
				break;
			default:
				break;

		}
		SDL_Flip(screen);
	}
	position.x = 0;
	position.y = 0;
	SDL_BlitSurface(screenTMP, NULL, screen, &position);
	SDL_Flip(screen);
	//On libère ce qui a été alloué dans la fonction.
	SDL_FreeSurface(screenTMP);
	SDL_FreeSurface(text);
	SDL_FreeSurface(menuEchap);

	deleteButton(button_Oui);
	deleteButton(button_Non);
}

SDL_Surface* menu_Help(SDL_Surface *screen, int WIDTH, int HEIGHT, TTF_Font *font, SDL_Color colorWrite, SDL_Color colorBackground){
	SDL_Surface *menuHelp = NULL;
	SDL_Surface *screenTMP = SDL_CreateRGBSurface(SDL_HWSURFACE, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
	SDL_Surface *text = NULL;
	SDL_Rect position;
	int widthMenu = 4*WIDTH / 5;
	int heightMenu = 5*HEIGHT / 8;

	position.x = 0;
	position.y = 0;
	SDL_BlitSurface(screen, NULL, screenTMP, &position);

	position.x = ((WIDTH - widthMenu) / 2);
	position.y = ((HEIGHT - heightMenu) / 2);

	menuHelp = SDL_CreateRGBSurface(SDL_HWSURFACE, widthMenu, heightMenu, 32, 0, 0, 0, 0);
	SDL_FillRect(menuHelp, NULL, SDL_MapRGB(screen->format, 77, 83, 84));
	SDL_BlitSurface(menuHelp, NULL, screen, &position);

	text = TTF_RenderText_Shaded(font, "Rules are simple. You're the red piece and", colorWrite, colorBackground);
	position.x = position.x + 5;
	position.y = position.y + 5;
	SDL_BlitSurface(text, NULL, screen, &position);

	text = TTF_RenderText_Shaded(font, "you need to go to the exit (the red line at the", colorWrite, colorBackground);
	position.y = position.y + text->h;
	SDL_BlitSurface(text, NULL, screen, &position);

	text = TTF_RenderText_Shaded(font, "board's edge). To do that, you need to move the", colorWrite, colorBackground);
	position.y = position.y + text->h;
	SDL_BlitSurface(text, NULL, screen, &position);

	text = TTF_RenderText_Shaded(font, "other pieces to free yourself a passage. Pieces", colorWrite, colorBackground);
	position.y = position.y + text->h;
	SDL_BlitSurface(text, NULL, screen, &position);

	text = TTF_RenderText_Shaded(font, "can't cross others or go outside the game area.", colorWrite, colorBackground);
	position.y = position.y + text->h;
	SDL_BlitSurface(text, NULL, screen, &position);

	text = TTF_RenderText_Shaded(font, "To move them, click on the desired piece, and", colorWrite, colorBackground);
	position.y = position.y + (text->h * 2);
	SDL_BlitSurface(text, NULL, screen, &position);

	text = TTF_RenderText_Shaded(font, "press the directional keys.", colorWrite, colorBackground);
	position.y = position.y + text->h;
	SDL_BlitSurface(text, NULL, screen, &position);

	text = TTF_RenderText_Shaded(font, "Good Luck !", colorWrite, colorBackground);
	position.y = position.y + text->h;
	SDL_BlitSurface(text, NULL, screen, &position);

	SDL_Flip(screen);
	return screenTMP;
}

// Meme principe que la fonction menu_echap. On affiche ce menu après avoir gagné une partie et si il y a un autre niveau à jouer.
int menu_continue(SDL_Surface *screen, int *check, int WIDTH, int HEIGHT, SDL_Color colorWrite, SDL_Color colorBackground, TTF_Font *font){
	SDL_Surface *screenTMP = SDL_CreateRGBSurface(SDL_HWSURFACE, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
	SDL_Surface *text = NULL;
	SDL_Surface *menuContinue = NULL;

	SDL_Color newColorHover = {149, 165, 166};
	SDL_Event event;
	SDL_Rect position;
	position.x = 0;
	position.y = 0;
	SDL_BlitSurface(screen, NULL, screenTMP, &position);

	text = TTF_RenderText_Blended(font, "Proceed to the next level ?", colorWrite);

	int widthMenu = text->w + 100;
	int heightMenu = HEIGHT/4;

	menuContinue = SDL_CreateRGBSurface(SDL_HWSURFACE, widthMenu,  heightMenu, 32, 0, 0, 0, 0);

	position.x = (WIDTH - (widthMenu))/2;
	position.y = (HEIGHT - (heightMenu))/2;
	SDL_FillRect(menuContinue, NULL, SDL_MapRGB(screen->format, 77, 83, 84)); //couleur Basalt
	SDL_BlitSurface(menuContinue, NULL, screen, &position);

	position.x = (position.x + ((position.x + widthMenu) - text->w))/2;
	position.y = (position.y + text->h);
	SDL_BlitSurface(text, NULL, screen, &position);

	SDL_FreeSurface(text);
	text = TTF_RenderText_Shaded(font, "   Yes   ", colorBackground, colorWrite);
	position.y = position.y - 3*text->h + heightMenu;

	button button_Yes = createButton(position.x, position.y, text->w, text->h);

	SDL_BlitSurface(text, NULL, screen, &position);


	SDL_FreeSurface(text);
	text = TTF_RenderText_Shaded(font, "   No   ", colorBackground, colorWrite);
	position.x += (widthMenu - 100) - text->w;

	button button_No = createButton(position.x, position.y, text->w, text->h);

	SDL_BlitSurface(text, NULL, screen, &position);

	int continueCheck = 1;
	int valReturn;
	int xMouse;
	int yMouse;

	while(continueCheck)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				continueCheck = 0;
				*check = 0;
				valReturn = 0;
				break;

			case SDL_MOUSEMOTION:
				xMouse = event.motion.x;
				yMouse = event.motion.y;
				if(checkButton(xMouse, yMouse, button_No))
					hoverButton(screen, "   No   ", button_No, colorWrite, newColorHover, font);
				else
					hoverButton(screen, "   No   ", button_No, colorBackground, colorWrite, font);

				if(checkButton(xMouse, yMouse, button_Yes))
					hoverButton(screen, "   Yes   ", button_Yes, colorWrite, newColorHover, font);
				else
					hoverButton(screen, "   Yes   ", button_Yes, colorBackground, colorWrite, font);
			break;

			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					xMouse = event.button.x;
					yMouse = event.button.y;
					if(checkButton(xMouse, yMouse, button_Yes))
					{
						continueCheck = 0;
						valReturn = 1;
					}
					if(checkButton(xMouse, yMouse, button_No))
					{
						continueCheck = 0;
						*check = 0;
						valReturn = 0;
					}
				}
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
					case SDLK_y:
						continueCheck = 0;
						valReturn = 1;
						break;

					case SDLK_n:
						continueCheck = 0;
						*check = 0;
						valReturn = 0;
						break;
					default:
						break;
				}
				break;

			default:
				break;

		}
		SDL_Flip(screen);
	}
	position.x = 0;
	position.y = 0;
	SDL_BlitSurface(screenTMP, NULL, screen, &position);
	SDL_Flip(screen);

	SDL_FreeSurface(screenTMP);
	SDL_FreeSurface(text);
	SDL_FreeSurface(menuContinue);

	deleteButton(button_Yes);
	deleteButton(button_No);

	return valReturn;

}

//Retourne si g rempli la condition de victoire du jeu selectionné.
bool game_over(game g){
	if(whatGame("rush-hour\n"))
		return game_over_hr(g);
	if(whatGame("klotski\n"))
		return game_over_ar(g);
	printf("game_over(), cas non prévu ...");
	return false;
}

//Permet de lancer un son de quelques secondes.
void son(Mix_Music* musique){
	if(musique != NULL)
		Mix_PlayMusic(musique, 0); //Jouer infiniment la musique
}

void deleteSon(Mix_Music *musique){
	if(musique != NULL)
		Mix_FreeMusic(musique);
}

// Permet d'afficher la fenetre principal, ainsi que le jeu.
void init_sdl_game(game g, int *MAIN_CHECK, int indGame){
	
	int NL = game_width(g);//Nombre de case de largeur
	int NH = game_height(g);//Nombre de case hauteur
	int SIZE_SQUARE = 75;//taille d'une case (CxC)

	int WIDTH = NL * SIZE_SQUARE + 250;//Largeur de l'écran
	int HEIGHT = NH * SIZE_SQUARE + 15;//Hauteur de l'écran

	SDL_Surface *screen = NULL;//Notre fenetre principale
	SDL_Surface *screenTMP = NULL;

	//On alloue la grille du jeu qui correspond au plateau du jeu.
	SDL_Surface ***grid = (SDL_Surface***) malloc(NH * sizeof(SDL_Surface**));
	SDL_Surface **grid2 = (SDL_Surface**)	malloc(NH * NL * sizeof(SDL_Surface*));

	if(grid == NULL)
		error("init_sdl_game(), Probleme d'allocation sur grid");
	if(grid2 == NULL)
		error("init_sdl_game(), Probleme d'allocation sur grid2");
	for(int i = 0; i < NH; i++)
		grid[i] = &grid2[i * NL];

	SDL_Surface *text = NULL;
	SDL_Surface *exitGame = NULL;//Un petit rectangle qui permet de localiser la fin du jeu.

	SDL_Rect position;//Variable position, elle nous permet de positionner les rectangles.
	SDL_Rect positionExit;//Sert uniquement à positionner le rectangle de victoire.

	SDL_Event event;
	TTF_Font *font = NULL;

	SDL_Init(SDL_INIT_VIDEO); // initialisation de la SDL

	SDL_Color colorBlack = {0, 0, 0}; //Couleur du texte -> noir
	SDL_Color colorWhite = {255,255,255}; // Blanc
	SDL_Color colorBasalt = {77, 83, 84};	// Basalt
	SDL_Color colorSilver = {189, 195, 199};
	SDL_Color colorGrey = {127, 140, 141};

	TTF_Init(); // Initialisation de TTF

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
	{	
		printf("%s", Mix_GetError());
	}	

	//Pour faire un plein screen il faut mettre SDL_FULLSCREEN
	screen = SDL_SetVideoMode(WIDTH - 1, HEIGHT - 1, 32, SDL_HWSURFACE | SDL_DOUBLEBUF); // On définit screen

	SDL_WM_SetCaption("Puzzle Games", NULL); // Titre de la fenetre
	SDL_WM_SetIcon(IMG_Load("truck.png"), NULL);

	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));

	font = TTF_OpenFont("Sansation-Regular.ttf", 20);

	text = TTF_RenderText_Shaded(font, "Puzzle Games", colorBlack, colorWhite);

	//On positionne le rectangle de victoire selon le jeu
	if(whatGame("rush-hour\n"))
	{
		exitGame = SDL_CreateRGBSurface(SDL_HWSURFACE, SIZE_SQUARE/6, SIZE_SQUARE - 1, 32, 0, 0, 0, 0);
		positionExit.x = SIZE_SQUARE * NL + 1;
		positionExit.y = abs(get_y(game_piece(g, 0)) - NH + 1) * SIZE_SQUARE;
	}
	else if(whatGame("klotski\n"))
	{
		exitGame = SDL_CreateRGBSurface(SDL_HWSURFACE, SIZE_SQUARE*2 - 2, SIZE_SQUARE/6, 32, 0, 0, 0, 0);
		positionExit.x = SIZE_SQUARE;
		positionExit.y = SIZE_SQUARE * NH + 1;
	}
	
	//On crée des rectangles que l'on stock dans le tableau 2D grille
	for(int y = 0; y < NH; y++)
		for(int x = 0; x < NL; x++)
			grid[y][x] = SDL_CreateRGBSurface(SDL_HWSURFACE, SIZE_SQUARE - 1, SIZE_SQUARE - 1, 32, 0, 0, 0, 0);

	//on affiche la grille du jeu avec les pieces.
	displayGridGame(g, screen, grid, NL, NH, SIZE_SQUARE, -1);

	//On affiche le rectangle de la victoire
	SDL_FillRect(exitGame, NULL, SDL_MapRGB(screen->format, 250, 20, 20));
	SDL_BlitSurface(exitGame, NULL, screen, &positionExit);

	//On affiche Puzzle Games
	position.x = (((NL * SIZE_SQUARE) + WIDTH) / 2) - (text->w / 2);
	position.y = 6/HEIGHT;
	SDL_BlitSurface(text, NULL, screen, &position);

	SDL_FreeSurface(text);
	// On affiche le nom du jeu précisement
	char* nameOfTheGame = whatGameStr();
	text = TTF_RenderText_Blended(font, nameOfTheGame, colorBlack);

	position.x = (((NL * SIZE_SQUARE) + WIDTH) / 2) - (text->w / 2);
	position.y = 6/HEIGHT + 20;
	SDL_BlitSurface(text, NULL, screen, &position);

	SDL_FreeSurface(text);

	text = TTF_RenderText_Shaded(font, "Number of moves :", colorBlack, colorWhite);
	position.x = (((NL * SIZE_SQUARE) + WIDTH) / 2) - (text->w / 2);
	position.y = position.y + 40;
	SDL_BlitSurface(text, NULL, screen, &position);
	SDL_FreeSurface(text);
	int yNbMove = position.y; //Permet de conserver l'ordonnée du text précedent afin d'afficher le nombre de mouvement.

	text = TTF_RenderText_Shaded(font, "   Help   ", colorBlack, colorSilver);
	position.x = (((NL * SIZE_SQUARE) + WIDTH) / 2) - (text->w / 2);
	position.y = 5*HEIGHT / 6;

	button button_Help = createButton(position.x, position.y, text->w, text->h);
	SDL_BlitSurface(text, NULL, screen, &position);

	Mix_Music *music_move = Mix_LoadMUS("move.mp3"); //Chargement de la musique;
	Mix_Music *music_wrongMove = Mix_LoadMUS("wrongMove.mp3"); //Chargement de la musique;

	int xMouse;
	int yMouse;
	int checkHelp = 0;
	int check = 1;//Condition principale de la boucle while.
	int indexPiece = -1;//On initialise cette variable à -1 afin qu'aucun move puisse être fait avant de cliquer sur une pièce.
	char NbMove[4];//Chaine de caractère affichant le nombre de mouvement.

	while(check && !game_over(g))
	{
		
		SDL_WaitEvent(&event); /* Récupération de l'événement dans event */
		switch(event.type) /* Test du type d'événement */
		{
			case SDL_QUIT: /* Si c'est un événement de type "Quitter" */
				check = 0;
				*MAIN_CHECK = 0;
				break;

			case SDL_MOUSEMOTION:
				xMouse = event.motion.x;
				yMouse = event.motion.y;
				if(checkButton(xMouse, yMouse, button_Help))
				{
					
					//(SDL_Surface *screen, char* str, button Button, SDL_Color colorWhite, SDL_Color newColor, TTF_Font *font){
					hoverButton(screen, "   Help   ", button_Help, colorBlack, colorGrey, font);
					//(SDL_Surface *screen, int WIDTH, int HEIGHT, TTF_Font *font, SDL_Color colorBlack)
					if(checkHelp == 0)
					{
						checkHelp = 1;
						screenTMP = menu_Help(screen, WIDTH, HEIGHT, font, colorWhite, colorBasalt);
					}
				}
				else
				{
					if(checkHelp == 1)
					{
						position.x = 0;
						position.y = 0;
						SDL_BlitSurface(screenTMP, NULL, screen, &position);
						checkHelp = 0;
					}
					hoverButton(screen, "   Help   ", button_Help, colorBlack,colorSilver, font);
				}

			case SDL_MOUSEBUTTONUP: /* Si c'est un evenement de type Clic Souris */
				if (checkHelp == 0 && (event.button.button == SDL_BUTTON_LEFT))//clic gauche de la souris
				{
					indexPiece = clic(event, g, WIDTH, HEIGHT, NL, NH, SIZE_SQUARE);
					displayGridGame(g, screen, grid, NL, NH, SIZE_SQUARE, indexPiece);
				}
				break;
			case SDL_KEYDOWN: /* Evenement de type touche enfoncée */
				switch(event.key.keysym.sym)
				{
					case SDLK_ESCAPE: //Touche Echap
						menu_echap(screen, MAIN_CHECK, &check, WIDTH, HEIGHT, colorWhite, colorBasalt, font);
						break;

					case SDLK_UP: //Touche fleche haut 
						if(checkHelp == 0 && indexPiece != -1 )
						{
							if(play_move(g, indexPiece, UP, 1))
								son(music_move);
							else
								son(music_wrongMove);
							displayGridGame(g, screen, grid, NL, NH, SIZE_SQUARE, indexPiece);
						}
						break;

					case SDLK_DOWN: //Touche fleche bas
						if(checkHelp == 0 && indexPiece != -1)
						{
							if(play_move(g, indexPiece, DOWN, 1))
								son(music_move);
							else
								son(music_wrongMove);
							displayGridGame(g, screen, grid, NL, NH, SIZE_SQUARE, indexPiece);
						}
						break;

					case SDLK_LEFT://Touche fleche gauche
						if(checkHelp == 0 && indexPiece != -1)
						{
							if(play_move(g, indexPiece, LEFT, 1))
								son(music_move);
							else
								son(music_wrongMove);
							displayGridGame(g, screen, grid, NL, NH, SIZE_SQUARE, indexPiece);
						}
						break;
					
					case SDLK_RIGHT://Touche fleche droite
						if(checkHelp == 0 && indexPiece != -1)
						{
							if(play_move(g, indexPiece, RIGHT, 1))
								son(music_move);
							else
								son(music_wrongMove);
							displayGridGame(g, screen, grid, NL, NH, SIZE_SQUARE, indexPiece);
						}
						break;

					default:
						break;
				}
				break;
			default:
				break;
		}

		if(checkHelp == 0)
		{
			SDL_FreeSurface(text);
			//On affiche une sorte de masque blanc afin d'éviter de lire l'ancien affichage du nombre de mouvement quand celui ci est plus petit que l'ancien.
			text = TTF_RenderText_Shaded(font, "xxxxxx", colorWhite, colorWhite);
			position.x = (((NL * SIZE_SQUARE) + WIDTH) / 2) - (text->w / 2);
			position.y = yNbMove + text->h;
			SDL_BlitSurface(text, NULL, screen, &position);

			SDL_FreeSurface(text);
			//On affiche le nombre de mouvement effectué
			sprintf(NbMove, "%d", game_nb_moves(g));
			text = TTF_RenderText_Shaded(font, NbMove, colorBlack, colorWhite);
			position.x = (((NL * SIZE_SQUARE) + WIDTH) / 2) - (text->w / 2);
			position.y = yNbMove + text->h;
			SDL_BlitSurface(text, NULL, screen, &position);
		}	
		SDL_Flip(screen);

		
	}

	//On regarde comment on est sorti de la boucle while.
	if(game_over(g))
	{
		Mix_Music *music_win = Mix_LoadMUS("win.mp3");
		son(music_win);
		clock_t arrivee=clock()+(0.4 * CLOCKS_PER_SEC); // On calcule le moment où l'attente devra s'arrêter
		while(clock() < arrivee);
		deleteSon(music_win);

		if(indGame < 3)
			menu_continue(screen, MAIN_CHECK, WIDTH, HEIGHT, colorWhite, colorBasalt, font);
	}

	deleteSon(music_move);
	deleteSon(music_wrongMove);
	Mix_CloseAudio(); //Fermeture de l'API
	//On libère les allocations.
	TTF_CloseFont(font);
	TTF_Quit();

	for(int y = 0; y < NH; y++)
		for(int x = 0; x < NL; x++)
			SDL_FreeSurface(grid[y][x]);
	free(grid[0]);
	free(grid);
	free(nameOfTheGame);
	SDL_FreeSurface(text);
	SDL_FreeSurface(exitGame);
	
	SDL_Quit();
}

void Apropos(SDL_Surface *screen, int WIDTH, int HEIGHT, SDL_Color colorWrite, SDL_Color colorBackground, TTF_Font *font){
	//on crée une surface ecran_tmp pour conserver l'ancien écran, afin de le réafficher à la fin de la fonction.
	SDL_Surface *screenTMP = SDL_CreateRGBSurface(SDL_HWSURFACE, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
	SDL_Surface *text = NULL;
	SDL_Surface *menuApropos = NULL;
	SDL_Color newColorHover = {149, 165, 166};

	SDL_Event event;
	SDL_Rect position;
	position.x = 0;
	position.y = 0;
	SDL_BlitSurface(screen, NULL, screenTMP, &position);

	//A partir d'ici, on va créer les deux bouttons Oui et Non, ainsi qu'afficher du text.
	text = TTF_RenderText_Blended(font, "A propos ...", colorWrite);

	int widthMenu = WIDTH;
	int heightMenu = HEIGHT;

	menuApropos = SDL_CreateRGBSurface(SDL_HWSURFACE, widthMenu,  heightMenu, 32, 0, 0, 0, 0);

	position.x = (WIDTH - (widthMenu))/2;
	position.y = (HEIGHT - (heightMenu))/2;
	SDL_FillRect(menuApropos, NULL, SDL_MapRGB(screen->format, 77, 83, 84)); //couleur Basalt
	SDL_BlitSurface(menuApropos, NULL, screen, &position);

	position.x = (position.x + ((position.x + widthMenu) - text->w))/2;
	position.y = (position.y + text->h);
	SDL_BlitSurface(text, NULL, screen, &position);

	position.y = position.y + 2 * text->h;
	text = TTF_RenderText_Shaded(font, "Puzzle Games Project (2015-2016) realized by", colorWrite, colorBackground);
	position.x = ((WIDTH - (widthMenu))/2) + 15;
	SDL_BlitSurface(text, NULL, screen, &position);

	position.y = position.y + text->h;
	text = TTF_RenderText_Shaded(font, "Halnaut Adrien, Marty Yoan and Ordonez Romain at", colorWrite, colorBackground);
	SDL_BlitSurface(text, NULL, screen, &position);

	position.y = position.y + text->h;
	text = TTF_RenderText_Shaded(font, "the University of Bordeaux. This binary is a", colorWrite, colorBackground);
	SDL_BlitSurface(text, NULL, screen, &position);

	position.y = position.y + text->h;
	text = TTF_RenderText_Shaded(font, "graphic version, using SDL-1.2, inspired by", colorWrite, colorBackground);
	SDL_BlitSurface(text, NULL, screen, &position);

	position.y = position.y + text->h;
	text = TTF_RenderText_Shaded(font, "games such as Rush Hour and Klotski.", colorWrite, colorBackground);
	SDL_BlitSurface(text, NULL, screen, &position);

	position.y = position.y + 2 * text->h;
	text = TTF_RenderText_Shaded(font, "   Back   ", colorBackground, colorWrite);
	position.x = (WIDTH - text->w) / 2;

	button button_Retour = createButton(position.x, position.y, text->w, text->h);

	SDL_BlitSurface(text, NULL, screen, &position);
	SDL_Flip(screen);
	//On commence la boucle while, avec la variable ci dessous comme condition.
	int checkApropos = 1;
	int xMouse;
	int yMouse;

	while(checkApropos)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_MOUSEMOTION:
				xMouse = event.motion.x;
				yMouse = event.motion.y;
				if (checkButton(xMouse, yMouse, button_Retour))
					hoverButton(screen, "   Back   ", button_Retour, colorWrite, newColorHover, font);
				else
					hoverButton(screen, "   Back   ", button_Retour, colorBackground, colorWrite, font);
			break;

			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					xMouse = event.button.x;
					yMouse = event.button.y;
					if(checkButton(xMouse, yMouse, button_Retour))
						checkApropos = 0;
				}
				break;
			default:
				break;

		}
		SDL_Flip(screen);
	}
	position.x = 0;
	position.y = 0;
	SDL_BlitSurface(screenTMP, NULL, screen, &position);
	SDL_Flip(screen);
	//On libère ce qui a été alloué dans la fonction.
	SDL_FreeSurface(screenTMP);
	SDL_FreeSurface(text);
	SDL_FreeSurface(menuApropos);

	deleteButton(button_Retour);

}

//Affiche le menu principal. Permet de choisir le jeu souhaité. Retourne -1 si l'utilisateur décide de quitter.
int choixDuJeu(){
	SDL_Surface *screen = NULL;
	SDL_Surface **text = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 4);
	SDL_Event event;

	SDL_Rect position;

	TTF_Font *font = NULL;

	int WIDTH = 480;
	int HEIGHT = 300;

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Color colorWrite = {0, 0, 0}; //Couleur du text -> noir
	SDL_Color colorBackground = {236, 240, 241};
	SDL_Color colorBasalt = {77, 83, 84};
	SDL_Color newColorHover = {192, 57, 43};

	TTF_Init();
	
	screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF); 
	
	SDL_WM_SetCaption("Puzzle Games", NULL);
	SDL_WM_SetIcon(IMG_Load("truck.png"), NULL);
	
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 236, 240, 241));
	
	font = TTF_OpenFont("Sansation-Regular.ttf", 20);

	TTF_SetFontStyle(font, TTF_STYLE_UNDERLINE);
	text[0] = TTF_RenderText_Shaded(font, "   Puzzle Games - Game Selection   ", colorWrite, colorBackground);

	position.x = (WIDTH - text[0]->w) / 2;
	position.y = HEIGHT / 8;

	SDL_BlitSurface(text[0], NULL, screen, &position);

	TTF_SetFontStyle(font, TTF_STYLE_NORMAL);

	//SDL_FreeSurface(text);
	text[1] = TTF_RenderText_Shaded(font, "   Rush Hour   ", colorBackground, colorWrite);

	position.x = (WIDTH - text[1]->w) / 2;
	position.y = (HEIGHT / 2) - text[1]->h;

	button button_Rh = createButton(position.x, position.y, text[1]->w, text[1]->h);

	SDL_BlitSurface(text[1], NULL, screen, &position);

	//SDL_FreeSurface(text);
	text[2] = TTF_RenderText_Shaded(font, "   Klotski   ", colorBackground, colorWrite);

	position.x = (WIDTH - text[2]->w) / 2;
	position.y = position.y + ( 2 * text[2]->h );

	button button_Ar = createButton(position.x, position.y, text[2]->w, text[2]->h);

	SDL_BlitSurface(text[2], NULL, screen, &position);

	//SDL_FreeSurface(text);
	text[3] = TTF_RenderText_Shaded(font, "   A propos ...   ", colorBackground, colorWrite);

	position.x = (WIDTH - text[3]->w) / 2;
	position.y = position.y + (2 * text[3]->h );

	button button_Apropos = createButton(position.x, position.y, text[3]->w, text[3]->h);

	SDL_BlitSurface(text[3], NULL, screen, &position);

	int check = 1;
	int xMouse;
	int yMouse;
	int tmp = 0;
	int valReturn;


	while(check)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				check = 0;
				valReturn = -1;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						
						menu_echap(screen, &tmp, &check, WIDTH, HEIGHT, colorBackground, colorBasalt, font);
						if(check == 0)
							valReturn = -1;
						break;
					default:
						break;
				}
				break;
			case SDL_MOUSEMOTION:
				xMouse = event.motion.x;
				yMouse = event.motion.y;
				if (checkButton(xMouse, yMouse, button_Rh))
					hoverButton(screen, "   Rush Hour   ", button_Rh, colorBackground, newColorHover, font);
				else
					hoverButton(screen, "   Rush Hour   ", button_Rh, colorBackground, colorWrite, font);

				if (checkButton(xMouse, yMouse, button_Ar))
					hoverButton(screen, "   Klotski   ", button_Ar, colorBackground, newColorHover, font);
				else
					hoverButton(screen, "   Klotski   ", button_Ar, colorBackground, colorWrite, font);
				
				if (checkButton(xMouse, yMouse, button_Apropos))
					hoverButton(screen, "   A propos ...   ", button_Apropos, colorBackground, newColorHover, font);
				else
					hoverButton(screen, "   A propos ...   ", button_Apropos, colorBackground, colorWrite, font);
				
			break;
			case SDL_MOUSEBUTTONUP:
				xMouse = event.button.x;
				yMouse = event.button.y;
				if(checkButton(xMouse, yMouse, button_Rh))
				{
					initFileConfig("rush-hour");
					check = 0;
				}
				if(checkButton(xMouse, yMouse, button_Ar))
				{
					initFileConfig("klotski");
					check = 0;
				}
				if(checkButton(xMouse, yMouse, button_Apropos))
				{
					//SDL_Surface *screen, int WIDTH, int HEIGHT, SDL_Color colorBackground, SDL_Color colorBasalt, TTF_Font *font
					Apropos(screen, WIDTH, HEIGHT, colorBackground, colorBasalt, font);
				}
				break;
			default:
				break;
		}

		SDL_Flip(screen);
	}


	TTF_CloseFont(font);
	TTF_Quit();
	for (int i = 0; i < 4; i++)
		SDL_FreeSurface(text[i]);

	SDL_Quit();

	deleteButton(button_Rh);
	deleteButton(button_Ar);
	deleteButton(button_Apropos);

	return valReturn;
}