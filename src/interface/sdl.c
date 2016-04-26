
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
	int new_x = x;
	int new_y = y;
	int new_w = width;
	int new_h = height;
	newButton->x = new_x;
	newButton->y = new_y;
	newButton->width = new_w;
	newButton->height = new_h;
	return newButton;
}

//Vérifie si les coordonnées (xMouse;yMouse) sont bien dans la surface du boutton "Button"
bool checkButton(int xMouse, int yMouse, button Button){
	if(Button == NULL)
		error("checkButton(), le bouton n'est pas alloué ou n'existe pas ...");
	//xMouse >= xOui && xMouse < (xOui + wOui) && yMouse >= yOui && yMouse < (yOui + hOui)
	return ( (xMouse >= (Button->x)) && (xMouse < ((Button->x) + (Button->width))) && (yMouse >= (Button->y)) && (yMouse < ((Button->y) + (Button->height))));
}

//Vérifie si les coordonnées (xMouse;yMouse) sont bien dans la surface du boutton "Button"
bool hoverButton(int xMouse, int yMouse, button Button){
	//if(Button == NULL)
	//	error("checkButton(), le bouton n'est pas alloué ou n'existe pas ...");
	//xMouse >= xOui && xMouse < (xOui + wOui) && yMouse >= yOui && yMouse < (yOui + hOui)
	return ( (xMouse >= (Button->x)) && (xMouse < ((Button->x) + (Button->width))) && (yMouse >= (Button->y)) && (yMouse < ((Button->y) + (Button->height))));
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
	file = fopen("../config.ini", "r");
	if(file == NULL)
		error("whatGameStr(), probleme d'ouverture du fichier config.ini");
	
	char* s = (char*)malloc(sizeof(char) * 256);
	fgets(s, 256, file);
	fclose(file);

	int i = 0;
	while(s[i] != '\0')
	{
		if(s[i] == '\n')
		{
			s[i] = '\0';
			return s;
		}
		i++;
	}
	return s;  
}
int min(int a, int b)
{
	if (a > b)
		return b;
	return a;
}
//Affiche a l'ecran un jeu "game g"
void afficherGrilleJeu(game g, SDL_Surface *ecran, SDL_Surface ***grille, int NL, int NH, int TAILLE_CASE, int selectedPiece){
	SDL_Rect position;
	//Ce tableau contient les couleurs que peuvent avoir une pièce. (r,g,b)
	int nb_couleurs = 11;
	int tab_couleurs[11][3] = {{231, 76, 60}, {255, 51, 153}, {155, 89, 182}, {52, 152, 219}, {46, 204, 113}, {52, 73, 94}, {241, 196, 15}, {230, 126, 34}, {246, 36, 89}, {51, 110, 123}, {31, 58, 147}};
	int decalZero = (!selectedPiece?0:1);
	int tab_c_selected[3] = {min(tab_couleurs[selectedPiece % nb_couleurs + decalZero][0]*1.2, 255), min(tab_couleurs[selectedPiece % nb_couleurs + decalZero][1] *1.2, 255), min(tab_couleurs[selectedPiece % nb_couleurs + decalZero][2] *1.2, 255)};
	//On parcours chaque rectangle, et pour chaque re)ctangle, on regarde son equivalent dans le jeu. Si le jeu renvoi -1 alors y'a rien, donc gris.
	//Sinon, le jeu va renvoyer l'indice associé au rectangle, et ainsi on va lui donner une couleur via le tableau.
	//La couleur d'indice 0 est réservée exclusivement pour la voiture principale du même indice, elle est la seule rouge.
	int i = 0;
	for(int y = 0; y < NH; y++)
	{
		for(int x = 0; x < NL; x++)
		{
			int indice_piece = game_square_piece(g, x, abs(y-NH +1));
			
			position.x = x*TAILLE_CASE + i;
			position.y = y*TAILLE_CASE + i;
			if(indice_piece != -1)
			{
				if(indice_piece == 0)
				{
					if (indice_piece != selectedPiece)
						SDL_FillRect(grille[y][x], NULL, SDL_MapRGB(ecran->format, tab_couleurs[0][0], tab_couleurs[0][1], tab_couleurs[0][2]));
					else
						SDL_FillRect(grille[y][x], NULL, SDL_MapRGB(ecran->format, tab_c_selected[0], tab_c_selected[1], tab_c_selected[2]));
				}
				else{
					if (indice_piece != selectedPiece)
						SDL_FillRect(grille[y][x], NULL, SDL_MapRGB(ecran->format, tab_couleurs[indice_piece%(nb_couleurs-1) + 1][0], tab_couleurs[indice_piece%(nb_couleurs-1) + 1][1], tab_couleurs[indice_piece%(nb_couleurs-1) + 1][2]));
					else
						SDL_FillRect(grille[y][x], NULL, SDL_MapRGB(ecran->format, tab_c_selected[0], tab_c_selected[1], tab_c_selected[2]));
				}
			}
			else
				SDL_FillRect(grille[y][x], NULL, SDL_MapRGB(ecran->format, 210, 210, 210));

			SDL_BlitSurface(grille[y][x], NULL, ecran, &position);
		}
	}
}

//Retourne l'indice de la pièce cliquée par la souris. -1 si le clic ne s'est pas effectué sur une pièce.
int clic(SDL_Event event, game g, int WIDTH, int HEIGHT, int NL, int NH, int TAILLE_CASE){
	//On recupere les coordonnées du clic
	int x_mouse = event.button.x;
	int y_mouse = event.button.y;

	//On convertit ces coordonnées en coordonnées plateau
	x_mouse = x_mouse / TAILLE_CASE;
	y_mouse = y_mouse / TAILLE_CASE;

	//on re oriente l'axe des ordonnées pour correspondre à l'orientation du jeu.
	y_mouse = abs(y_mouse-NH +1);

	//On regarde si le clic s'est bien produit sur le plateau
	if(x_mouse < 0 || x_mouse >= NL || y_mouse < 0 || y_mouse >= NH)
		return -1;
	//ici les coordonnées sont valides donc on peut les manipuler.

	int indice_piece = game_square_piece(g, x_mouse, y_mouse);
	if(indice_piece < 0)
		return -1;
	return indice_piece;
}

//Affiche un petit menu echap lorsqu'on appuie sur la touche echap.
void menu_echap(SDL_Surface *ecran,int *continuer_principal, int *continuer, int WIDTH, int HEIGHT, SDL_Color couleurFond, SDL_Color couleurBasalt, TTF_Font *police){
	//on crée une surface ecran_tmp pour conserver l'ancien écran, afin de le réafficher à la fin de la fonction.
	SDL_Surface *ecran_tmp = SDL_CreateRGBSurface(SDL_HWSURFACE, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
	SDL_Surface *texte = NULL;
	SDL_Surface *menu_echap = NULL;

	SDL_Event event;
	SDL_Rect position;
	position.x = 0;
	position.y = 0;
	SDL_BlitSurface(ecran, NULL, ecran_tmp, &position);

	//A partir d'ici, on va créer les deux bouttons Oui et Non, ainsi qu'afficher du texte.
	texte = TTF_RenderText_Blended(police, "Voulez vous quitter le jeu ?", couleurFond);

	int w_echap = texte->w + 100;
	int h_echap = HEIGHT/3;

	menu_echap = SDL_CreateRGBSurface(SDL_HWSURFACE, w_echap,  h_echap, 32, 0, 0, 0, 0);

	position.x = (WIDTH - (w_echap))/2;
	position.y = (HEIGHT - (h_echap))/2;
	SDL_FillRect(menu_echap, NULL, SDL_MapRGB(ecran->format, 77, 83, 84)); //couleur Basalt
	SDL_BlitSurface(menu_echap, NULL, ecran, &position);

	position.x = (position.x + ((position.x + w_echap) - texte->w))/2;
	position.y = (position.y + texte->h);
	SDL_BlitSurface(texte, NULL, ecran, &position);

	SDL_FreeSurface(texte);
	texte = TTF_RenderText_Shaded(police, "Oui", couleurBasalt, couleurFond);
	position.y = position.y - 3*texte->h + h_echap;

	button button_Oui = createButton(position.x, position.y, texte->w, texte->h);

	SDL_BlitSurface(texte, NULL, ecran, &position);

	SDL_FreeSurface(texte);
	texte = TTF_RenderText_Shaded(police, "Non", couleurBasalt, couleurFond);
	position.x += (w_echap - 100) - texte->w;

	button button_Non = createButton(position.x, position.y, texte->w, texte->h);

	SDL_BlitSurface(texte, NULL, ecran, &position);

	//On commence la boucle while, avec la variable ci dessous comme condition.
	int continuer_echap = 1;

	while(continuer_echap)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				continuer_echap = 0;
				*continuer = 0;
				*continuer_principal = 0;
				break;

			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					int xMouse = event.button.x;
					int yMouse = event.button.y;
					if(checkButton(xMouse, yMouse, button_Oui))
					{
						continuer_echap = 0;
						*continuer = 0;
						*continuer_principal = 0;
					}
					if(checkButton(xMouse, yMouse, button_Non))
						continuer_echap = 0;
				}
				break;

			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						continuer_echap = 0;
						break;
					case SDLK_o:
						*continuer = 0;
						continuer_echap = 0;
						break;
					case SDLK_n:
						continuer_echap = 0;
					default:
						break;
				}
				break;
			default:
				break;

		}
		SDL_Flip(ecran);
	}
	position.x = 0;
	position.y = 0;
	SDL_BlitSurface(ecran_tmp, NULL, ecran, &position);
	SDL_Flip(ecran);
	//On libère ce qui a été alloué dans la fonction.
	SDL_FreeSurface(ecran_tmp);
	SDL_FreeSurface(texte);
	SDL_FreeSurface(menu_echap);

	deleteButton(button_Oui);
	deleteButton(button_Non);
}



// Meme principe que la fonction menu_echap. On affiche ce menu après avoir gagné une partie et si il y a un autre niveau à jouer.
int menu_continuer(SDL_Surface *ecran, int *continuer, int WIDTH, int HEIGHT, SDL_Color couleurFond, SDL_Color couleurBasalt, TTF_Font *police){
	SDL_Surface *ecran_tmp = SDL_CreateRGBSurface(SDL_HWSURFACE, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
	SDL_Surface *texte = NULL;
	SDL_Surface *menu_continuer = NULL;

	SDL_Event event;
	SDL_Rect position;
	position.x = 0;
	position.y = 0;
	SDL_BlitSurface(ecran, NULL, ecran_tmp, &position);

	texte = TTF_RenderText_Blended(police, "Voulez vous continuer ?", couleurFond);

	int w_continuer = texte->w + 100;
	int h_continuer = HEIGHT/4;

	menu_continuer = SDL_CreateRGBSurface(SDL_HWSURFACE, w_continuer,  h_continuer, 32, 0, 0, 0, 0);

	position.x = (WIDTH - (w_continuer))/2;
	position.y = (HEIGHT - (h_continuer))/2;
	SDL_FillRect(menu_continuer, NULL, SDL_MapRGB(ecran->format, 77, 83, 84)); //couleur Basalt
	SDL_BlitSurface(menu_continuer, NULL, ecran, &position);

	position.x = (position.x + ((position.x + w_continuer) - texte->w))/2;
	position.y = (position.y + texte->h);
	SDL_BlitSurface(texte, NULL, ecran, &position);

	SDL_FreeSurface(texte);
	texte = TTF_RenderText_Shaded(police, "Oui", couleurBasalt, couleurFond);
	position.y = position.y - 3*texte->h + h_continuer;

	button button_Oui = createButton(position.x, position.y, texte->w, texte->h);

	SDL_BlitSurface(texte, NULL, ecran, &position);


	SDL_FreeSurface(texte);
	texte = TTF_RenderText_Shaded(police, "Non", couleurBasalt, couleurFond);
	position.x += (w_continuer - 100) - texte->w;

	button button_Non = createButton(position.x, position.y, texte->w, texte->h);

	SDL_BlitSurface(texte, NULL, ecran, &position);

	int continuer_check = 1;
	int valeur_retour;

	while(continuer_check)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				continuer_check = 0;
				*continuer = 0;
				valeur_retour = 0;
				break;

			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					int xMouse = event.button.x;
					int yMouse = event.button.y;
					if(checkButton(xMouse, yMouse, button_Oui))
					{
						continuer_check = 0;
						valeur_retour = 1;
					}
					if(checkButton(xMouse, yMouse, button_Non))
					{
						continuer_check = 0;
						*continuer = 0;
						valeur_retour = 0;
					}
				}
				break;

			default:
				break;

		}
		SDL_Flip(ecran);
	}
	position.x = 0;
	position.y = 0;
	SDL_BlitSurface(ecran_tmp, NULL, ecran, &position);
	SDL_Flip(ecran);

	SDL_FreeSurface(ecran_tmp);
	SDL_FreeSurface(texte);
	SDL_FreeSurface(menu_continuer);

	deleteButton(button_Oui);
	deleteButton(button_Non);

	return valeur_retour;

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
	
	// Mix_Music *musique; //Création du pointeur de type Mix_Music
	// musique = Mix_LoadMUS(nom); //Chargement de la musique
	Mix_PlayMusic(musique, 0); //Jouer infiniment la musique

	//Pause de quelques secondes, juste pour que le son soit joué
	// clock_t arrivee=clock()+(temps * CLOCKS_PER_SEC); // On calcule le moment où l'attente devra s'arrêter
	// while(clock() < arrivee);

}

void deleteSon(Mix_Music *musique){
	if(musique != NULL)
		Mix_FreeMusic(musique);
}

// Permet d'afficher la fenetre principal, ainsi que le jeu.
void init_sdl_game(game g, int *continuer_principal, int indGame){
	
	int NL = game_width(g);//Nombre de case de largeur
	int NH = game_height(g);//Nombre de case hauteur
	int TAILLE_CASE = 75;//taille d'une case (CxC)

	int WIDTH = NL * TAILLE_CASE + 250;//Largeur de l'écran
	int HEIGHT = NH * TAILLE_CASE + 15;//Hauteur de l'écran

	SDL_Surface *ecran = NULL;//Notre fenetre principale

	//On alloue la grille du jeu qui correspond au plateau du jeu.
	SDL_Surface ***grille = (SDL_Surface***) malloc(NH * sizeof(SDL_Surface**));
	SDL_Surface **grille2 = (SDL_Surface**)	malloc(NH * NL * sizeof(SDL_Surface*));

	if(grille == NULL)
		error("init_sdl_game(), Probleme d'allocation sur grille");
	if(grille2 == NULL)
		error("init_sdl_game(), Probleme d'allocation sur grille2");
	for(int i = 0; i < NH; i++)
		grille[i] = &grille2[i * NL];

	SDL_Surface *texte = NULL;
	SDL_Surface *sortie_jeu = NULL;//Un petit rectangle qui permet de localiser la fin du jeu.

	SDL_Rect position;//Variable position, elle nous permet de positionner les rectangles.
	SDL_Rect position_fin;//Sert uniquement à positionner le rectangle de victoire.

	SDL_Event event;
	TTF_Font *police = NULL;

	SDL_Init(SDL_INIT_VIDEO); // initialisation de la SDL

	SDL_Color couleurEcriture = {0, 0, 0}; //Couleur du texte -> noir
	SDL_Color couleurFond = {255,255,255}; // Blanc
	SDL_Color couleurBasalt = {77, 83, 84};	// Basalt

	TTF_Init(); // Initialisation de TTF

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
	{	
		printf("%s", Mix_GetError());
	}	

	//Pour faire un plein ecran il faut mettre SDL_FULLSCREEN
	ecran = SDL_SetVideoMode(WIDTH - 1, HEIGHT - 1, 32, SDL_HWSURFACE | SDL_DOUBLEBUF); // On définit Ecran

	SDL_WM_SetCaption("Puzzle Games", NULL); // Titre de la fenetre

	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));

	police = TTF_OpenFont("Sansation-Regular.ttf", 20);

	texte = TTF_RenderText_Shaded(police, "Puzzle Games", couleurEcriture, couleurFond);

	//On positionne le rectangle de victoire selon le jeu
	if(whatGame("rush-hour\n"))
	{
		sortie_jeu = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE_CASE/6, TAILLE_CASE - 1, 32, 0, 0, 0, 0);
		position_fin.x = TAILLE_CASE * NL + 1;
		position_fin.y = abs(get_y(game_piece(g, 0)) - NH + 1) * TAILLE_CASE;
	}
	else if(whatGame("klotski\n"))
	{
		sortie_jeu = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE_CASE*2 - 2, TAILLE_CASE/6, 32, 0, 0, 0, 0);
		position_fin.x = TAILLE_CASE;
		position_fin.y = TAILLE_CASE * NH + 1;
	}
	
	//On crée des rectangles que l'on stock dans le tableau 2D grille
	for(int y = 0; y < NH; y++)
		for(int x = 0; x < NL; x++)
			grille[y][x] = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE_CASE - 1, TAILLE_CASE - 1, 32, 0, 0, 0, 0);

	//on affiche la grille du jeu avec les pieces.
	afficherGrilleJeu(g, ecran, grille, NL, NH, TAILLE_CASE, -1);

	//On affiche le rectangle de la victoire
	SDL_FillRect(sortie_jeu, NULL, SDL_MapRGB(ecran->format, 250, 20, 20));
	SDL_BlitSurface(sortie_jeu, NULL, ecran, &position_fin);

	//On affiche Puzzle Games
	position.x = (((NL * TAILLE_CASE) + WIDTH) / 2) - (texte->w / 2);
	position.y = 6/HEIGHT;
	SDL_BlitSurface(texte, NULL, ecran, &position);

	SDL_FreeSurface(texte);
	// On affiche le nom du jeu précisement
	char* nomDuJeu = whatGameStr();
	texte = TTF_RenderText_Blended(police, nomDuJeu, couleurEcriture);

	position.x = (((NL * TAILLE_CASE) + WIDTH) / 2) - (texte->w / 2);
	position.y = 6/HEIGHT + 20;
	SDL_BlitSurface(texte, NULL, ecran, &position);

	SDL_FreeSurface(texte);
	texte = TTF_RenderText_Shaded(police, "Nombre de Deplacement:", couleurEcriture, couleurFond);
	position.x = (((NL * TAILLE_CASE) + WIDTH) / 2) - (texte->w / 2);
	position.y = position.y + 40;
	SDL_BlitSurface(texte, NULL, ecran, &position);

	int yNbMove = position.y; //Permet de conserver l'ordonnée du texte précedent afin d'afficher le nombre de mouvement.

	Mix_Music *music_move = Mix_LoadMUS("move.mp3"); //Chargement de la musique;
	Mix_Music *music_wrongMove = Mix_LoadMUS("wrongMove.mp3"); //Chargement de la musique;

	int continuer = 1;//Condition principale de la boucle while.
	int indice_piece = -1;//On initialise cette variable à -1 afin qu'aucun move puisse être fait avant de cliquer sur une pièce.
	char NbMove[4];//Chaine de caractère affichant le nombre de mouvement.

	while(continuer && !game_over(g))
	{
		
		SDL_WaitEvent(&event); /* Récupération de l'événement dans event */
		switch(event.type) /* Test du type d'événement */
		{
			case SDL_QUIT: /* Si c'est un événement de type "Quitter" */
				continuer = 0;
				*continuer_principal = 0;
				break;

			case SDL_MOUSEBUTTONUP: /* Si c'est un evenement de type Clic Souris */
				if (event.button.button == SDL_BUTTON_LEFT)//clic gauche de la souris
				{
					indice_piece = clic(event, g, WIDTH, HEIGHT, NL, NH, TAILLE_CASE);
					afficherGrilleJeu(g, ecran, grille, NL, NH, TAILLE_CASE, indice_piece);
				}
				break;
			case SDL_MOUSEMOTION: /*Mouvement de souris*/

				break;
			case SDL_KEYDOWN: /* Evenement de type touche enfoncée */
				switch(event.key.keysym.sym)
				{
					case SDLK_ESCAPE: //Touche Echap
						menu_echap(ecran, continuer_principal, &continuer, WIDTH, HEIGHT, couleurFond, couleurBasalt, police);
						break;

					case SDLK_UP: //Touche fleche haut 
						if(indice_piece != -1)
						{
							if(play_move(g, indice_piece, UP, 1))
								son(music_move);
							else
								son(music_wrongMove);
							afficherGrilleJeu(g, ecran, grille, NL, NH, TAILLE_CASE, indice_piece);
						}
						break;

					case SDLK_DOWN: //Touche fleche bas
						if(indice_piece != -1)
						{
							if(play_move(g, indice_piece, DOWN, 1))
								son(music_move);
							else
								son(music_wrongMove);
							afficherGrilleJeu(g, ecran, grille, NL, NH, TAILLE_CASE, indice_piece);
						}
						break;

					case SDLK_LEFT://Touche fleche gauche
						if(indice_piece != -1)
						{
							if(play_move(g, indice_piece, LEFT, 1))
								son(music_move);
							else
								son(music_wrongMove);
							afficherGrilleJeu(g, ecran, grille, NL, NH, TAILLE_CASE, indice_piece);
						}
						break;
					
					case SDLK_RIGHT://Touche fleche droite
						if(indice_piece != -1)
						{
							if(play_move(g, indice_piece, RIGHT, 1))
								son(music_move);
							else
								son(music_wrongMove);
							afficherGrilleJeu(g, ecran, grille, NL, NH, TAILLE_CASE, indice_piece);
						}
						break;

					default:
						break;
				}
				break;
			default:
				break;
		}

		SDL_FreeSurface(texte);
		//On affiche une sorte de masque blanc afin d'éviter de lire l'ancien affichage du nombre de mouvement quand celui ci est plus petit que l'ancien.
		texte = TTF_RenderText_Shaded(police, "xxxxxx", couleurFond, couleurFond);
		position.x = (((NL * TAILLE_CASE) + WIDTH) / 2) - (texte->w / 2);
		position.y = yNbMove + texte->h;
		SDL_BlitSurface(texte, NULL, ecran, &position);

		SDL_FreeSurface(texte);
		//On affiche le nombre de mouvement effectué
		sprintf(NbMove, "%d", game_nb_moves(g));
		texte = TTF_RenderText_Shaded(police, NbMove, couleurEcriture, couleurFond);
		position.x = (((NL * TAILLE_CASE) + WIDTH) / 2) - (texte->w / 2);
		position.y = yNbMove + texte->h;
		SDL_BlitSurface(texte, NULL, ecran, &position);

		SDL_Flip(ecran);
		
	}

	//On regarde comment on est sorti de la boucle while.
	if(game_over(g))
	{
		Mix_Music *music = Mix_LoadMUS("win.mp3");
		son(music);
		clock_t arrivee=clock()+(0.4 * CLOCKS_PER_SEC); // On calcule le moment où l'attente devra s'arrêter
		while(clock() < arrivee);
		deleteSon(music);

		if(indGame < 3)
		{
			menu_continuer(ecran, continuer_principal, WIDTH, HEIGHT, couleurFond, couleurBasalt, police);
		}
	}

	deleteSon(music_move);
	deleteSon(music_wrongMove);
	Mix_CloseAudio(); //Fermeture de l'API
	//On libère les allocations.
	TTF_CloseFont(police);
	TTF_Quit();

	for(int y = 0; y < NH; y++)
		for(int x = 0; x < NL; x++)
			SDL_FreeSurface(grille[y][x]);
	free(grille[0]);
	free(grille);
	free(nomDuJeu);
	SDL_FreeSurface(texte);
	SDL_FreeSurface(sortie_jeu);
	
	SDL_Quit();
}


void Apropos(SDL_Surface *ecran, int WIDTH, int HEIGHT, SDL_Color couleurFond, SDL_Color couleurBasalt, TTF_Font *police){
	//on crée une surface ecran_tmp pour conserver l'ancien écran, afin de le réafficher à la fin de la fonction.
	SDL_Surface *ecran_tmp = SDL_CreateRGBSurface(SDL_HWSURFACE, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
	SDL_Surface *texte = NULL;
	SDL_Surface *menu_apropos = NULL;

	menu_apropos = IMG_Load("a_propos.png");

	SDL_Event event;
	SDL_Rect position;
	position.x = 0;
	position.y = 0;
	SDL_BlitSurface(ecran, NULL, ecran_tmp, &position);

	SDL_BlitSurface(menu_apropos, NULL, ecran, &position);
	
	texte = TTF_RenderText_Shaded(police, "Retour", couleurBasalt, couleurFond);
	position.x = 210;
	position.y = 240;

	button button_Retour = createButton(position.x, position.y, texte->w, texte->h);

	SDL_BlitSurface(texte, NULL, ecran, &position);
	SDL_Flip(ecran);

	//On commence la boucle while, avec la variable ci dessous comme condition.
	int continuer_Apropos = 1;

	while(continuer_Apropos)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{

			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					int xMouse = event.button.x;
					int yMouse = event.button.y;
					if(checkButton(xMouse, yMouse, button_Retour))
						continuer_Apropos = 0;
				}
				break;
			default:
				break;

		}
		SDL_Flip(ecran);
	}
	position.x = 0;
	position.y = 0;
	SDL_BlitSurface(ecran_tmp, NULL, ecran, &position);
	SDL_Flip(ecran);
	//On libère ce qui a été alloué dans la fonction.
	SDL_FreeSurface(ecran_tmp);
	SDL_FreeSurface(texte);
	SDL_FreeSurface(menu_apropos);

	deleteButton(button_Retour);

}

//Affiche le menu principal. Permet de choisir le jeu souhaité. Retourne -1 si l'utilisateur décide de quitter.
int choixDuJeu(){
	SDL_Surface *ecran = NULL;
	SDL_Surface **texte = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 4);
	SDL_Event event;

	SDL_Rect position;

	TTF_Font *police = NULL;

	int WIDTH = 480;
	int HEIGHT = 300;

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Color couleurEcriture = {0, 0, 0}; //Couleur du texte -> noir
	SDL_Color couleurFond = {236, 240, 241};
	SDL_Color couleurBasalt = {77, 83, 84};

	TTF_Init();
	
	ecran = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF); 
	
	SDL_WM_SetCaption("Puzzle Games", NULL);
	
	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 236, 240, 241));
	
	police = TTF_OpenFont("Sansation-Regular.ttf", 20);

	TTF_SetFontStyle(police, TTF_STYLE_UNDERLINE);
	texte[0] = TTF_RenderText_Shaded(police, "   Puzzle Games - Choix du Jeu   ", couleurEcriture, couleurFond);

	position.x = (WIDTH - texte[0]->w) / 2;
	position.y = HEIGHT / 8;

	SDL_BlitSurface(texte[0], NULL, ecran, &position);

	TTF_SetFontStyle(police, TTF_STYLE_NORMAL);

	//SDL_FreeSurface(texte);
	texte[1] = TTF_RenderText_Shaded(police, "   Rush Hour   ", couleurFond, couleurEcriture);

	position.x = (WIDTH - texte[1]->w) / 2;
	position.y = (HEIGHT / 2) - texte[1]->h;

	button button_Rh = createButton(position.x, position.y, texte[0]->w, texte[0]->h);

	SDL_BlitSurface(texte[1], NULL, ecran, &position);

	//SDL_FreeSurface(texte);
	texte[2] = TTF_RenderText_Shaded(police, "   Klotski   ", couleurFond, couleurEcriture);

	position.x = (WIDTH - texte[2]->w) / 2;
	position.y = position.y + ( 2 * texte[2]->h );

	button button_Ar = createButton(position.x, position.y, texte[0]->w, texte[0]->h);

	SDL_BlitSurface(texte[2], NULL, ecran, &position);

	//SDL_FreeSurface(texte);
	texte[3] = TTF_RenderText_Shaded(police, "   A propos ...   ", couleurFond, couleurEcriture);

	position.x = (WIDTH - texte[3]->w) / 2;
	position.y = position.y + (2 * texte[3]->h );

	button button_Apropos = createButton(position.x, position.y, texte[0]->w, texte[0]->h);

	SDL_BlitSurface(texte[3], NULL, ecran, &position);

	int continuer = 1;
	int xMouse;
	int yMouse;
	int tmp = 0;
	int valeur_retour;


	while(continuer)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				continuer = 0;
				valeur_retour = -1;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						
						menu_echap(ecran, &tmp, &continuer, WIDTH, HEIGHT, couleurFond, couleurBasalt, police);
						if(continuer == 0)
							valeur_retour = -1;
						break;
					default:
						break;
				}
				break;
			case SDL_MOUSEMOTION:
				xMouse = event.motion.x;
				yMouse = event.motion.y;
				if (checkButton(xMouse, yMouse, button_Rh))
				{
					SDL_Color couleurmagique = {255,17,17};
					texte[1] = TTF_RenderText_Shaded(police, "   Rush Hour   ", couleurFond, couleurmagique);
					position.x = button_Rh->x;
					position.y = button_Rh->y;
					SDL_BlitSurface(texte[1], NULL, ecran, &position);
				}else{
					SDL_Color couleurmagique = {0,0,0};
					texte[1] = TTF_RenderText_Shaded(police, "   Rush Hour   ", couleurFond, couleurmagique);
					position.x = button_Rh->x;
					position.y = button_Rh->y;
					SDL_BlitSurface(texte[1], NULL, ecran, &position);
				}
				if (checkButton(xMouse, yMouse, button_Ar))
				{
					SDL_Color couleurmagique = {255,17,17};
					texte[2] = TTF_RenderText_Shaded(police, "   Klotski   ", couleurFond, couleurmagique);
					position.x = button_Ar->x;
					position.y = button_Ar->y;
					SDL_BlitSurface(texte[2], NULL, ecran, &position);
				}else{
					SDL_Color couleurmagique = {0,0,0};
					texte[2] = TTF_RenderText_Shaded(police, "   Klotski   ", couleurFond, couleurmagique);
					position.x = button_Ar->x;
					position.y = button_Ar->y;
					SDL_BlitSurface(texte[2], NULL, ecran, &position);
				}
				if (checkButton(xMouse, yMouse, button_Apropos))
				{
					SDL_Color couleurmagique = {255,17,17};
					texte[3] = TTF_RenderText_Shaded(police, "   A propos ...   ", couleurFond, couleurmagique);
					position.x = button_Apropos->x;
					position.y = button_Apropos->y;
					SDL_BlitSurface(texte[3], NULL, ecran, &position);
				}else{
					SDL_Color couleurmagique = {0,0,0};
					texte[3] = TTF_RenderText_Shaded(police, "   A propos ...   ", couleurFond, couleurmagique);
					position.x = button_Apropos->x;
					position.y = button_Apropos->y;
					SDL_BlitSurface(texte[3], NULL, ecran, &position);
				}
			break;
			case SDL_MOUSEBUTTONUP:
				xMouse = event.button.x;
				yMouse = event.button.y;
				if(checkButton(xMouse, yMouse, button_Rh))
				{
					initFileConfig("rush-hour");
					continuer = 0;
				}
				if(checkButton(xMouse, yMouse, button_Ar))
				{
					initFileConfig("klotski");
					continuer = 0;
				}
				if(checkButton(xMouse, yMouse, button_Apropos))
				{
					//SDL_Surface *ecran, int WIDTH, int HEIGHT, SDL_Color couleurFond, SDL_Color couleurBasalt, TTF_Font *police
					Apropos(ecran, WIDTH, HEIGHT, couleurFond, couleurBasalt, police);
				}
				break;
			default:
				break;
		}

		SDL_Flip(ecran);
	}


	TTF_CloseFont(police);
	TTF_Quit();
	for (int i = 0; i < 4; i++)
		SDL_FreeSurface(texte[i]);

	SDL_Quit();

	deleteButton(button_Rh);
	deleteButton(button_Ar);
	deleteButton(button_Apropos);

	return valeur_retour;
}
