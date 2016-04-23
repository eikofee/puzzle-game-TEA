#include <stdlib.h>
#include <stdio.h>
#include <time.h>
// #include <sdl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

// #include <game.h>
#include <game_ar.h>
#include <game_rh.h>
#include <utility.h>
#include <interface_txt.h>

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

//Une boucle infini qui utilise WaitEvent. En gros, ca boucle, et ca attend qu'on clique sur la croix :-)
void pause(){
	int continuer = 1;
	SDL_Event event;

	while(continuer)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				continuer = 0;
		}
	}
}

void afficherGrilleJeu(game g, SDL_Surface *ecran, SDL_Surface ***grille, int NL, int NH, int TAILLE_CASE){
	SDL_Rect position;
	int nb_couleurs = 9;
	int tab_couleurs[9][3] = {{231, 76, 60}, {255, 51, 153}, {155, 89, 182}, {52, 152, 219}, {46, 204, 113}, {52, 73, 94}, {241, 196, 15}, {230, 126, 34}, {255, 51, 153}};

	//On parcours chaque rectangle, et pour chaque rectangle, on regarde son equivalent dans le jeu. Si le jeu renvoi -1 alors y'a rien, donc gris.
	//Sinon, le jeu va renvoyer l'indice associé au rectangle, et ainsi on va lui donner une couleur via le tableau.
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
					SDL_FillRect(grille[y][x], NULL, SDL_MapRGB(ecran->format, tab_couleurs[0][0], tab_couleurs[0][1], tab_couleurs[0][2]));
				else
					SDL_FillRect(grille[y][x], NULL, SDL_MapRGB(ecran->format, tab_couleurs[indice_piece%(nb_couleurs-1) + 1][0], tab_couleurs[indice_piece%(nb_couleurs-1) + 1][1], tab_couleurs[indice_piece%(nb_couleurs-1) + 1][2]));
			}
			else
			{
				SDL_FillRect(grille[y][x], NULL, SDL_MapRGB(ecran->format, 210, 210, 210));
			}
			SDL_BlitSurface(grille[y][x], NULL, ecran, &position);
			
		}
	}

}

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
	{
		// printf("Les coordonnées du clic sont en dehors du plateau de jeu.\n\n");
		return -1;
	}
	//ici les coordonnées sont valides donc on peut les manipuler.
	
	// printf("x: %d\ny: %d\n\n", x_mouse, y_mouse);

	int indice_piece = game_square_piece(g, x_mouse, y_mouse);
	if(indice_piece < 0)
		return -1;
	return indice_piece;
}


void menu_echap(SDL_Surface *ecran, int *continuer, int WIDTH, int HEIGHT, SDL_Color couleurFond, SDL_Color couleurBasalt, TTF_Font *police){
	SDL_Surface *ecran_tmp = SDL_CreateRGBSurface(SDL_HWSURFACE, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
	SDL_Surface *texte = NULL;
	SDL_Surface *menu_echap = NULL;

	SDL_Event event;
	SDL_Rect position;
	position.x = 0;
	position.y = 0;
	SDL_BlitSurface(ecran, NULL, ecran_tmp, &position);

	texte = TTF_RenderText_Blended(police, "Voulez vous quitter le jeu ?", couleurFond);

	int w_echap = texte->w + 100;
	int h_echap = HEIGHT/4;

	menu_echap = SDL_CreateRGBSurface(SDL_HWSURFACE, w_echap,  h_echap, 32, 0, 0, 0, 0);

	position.x = (WIDTH - (w_echap))/2;
	position.y = (HEIGHT - (h_echap))/2;
	SDL_FillRect(menu_echap, NULL, SDL_MapRGB(ecran->format, 77, 83, 84)); //couleur Basalt
	SDL_BlitSurface(menu_echap, NULL, ecran, &position);

    position.x = (position.x + ((position.x + w_echap) - texte->w))/2;
    position.y = (position.y + texte->h);
    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "Oui", couleurBasalt, couleurFond);
    position.y = position.y - 3*texte->h + h_echap;

	int xOui = position.x;
	int yOui = position.y;
	int wOui = texte->w;
	int hOui = texte->h;

    SDL_BlitSurface(texte, NULL, ecran, &position);

    texte = TTF_RenderText_Shaded(police, "Non", couleurBasalt, couleurFond);
    position.x += (w_echap - 100) - texte->w;

    int xNon = position.x;
    int yNon = position.y;
    int wNon = texte->w;
    int hNon = texte->h;

    SDL_BlitSurface(texte, NULL, ecran, &position);

    int continuer_echap = 1;

    while(continuer_echap)
    {
    	SDL_WaitEvent(&event);
    	switch(event.type)
    	{
    		case SDL_QUIT:
    			continuer_echap = 0;
    			continuer = 0;
    			break;

			case SDL_MOUSEBUTTONUP:
		        if (event.button.button == SDL_BUTTON_LEFT)
		        {
		        	int xMouse = event.button.x;
		        	int yMouse = event.button.y;
		        	if(xMouse >= xOui && xMouse < (xOui + wOui) && yMouse >= yOui && yMouse < (yOui + hOui))
		        	{
		        		continuer_echap = 0;
		        		*continuer = 0;
		        	}
		        	if(xMouse >= xNon && xMouse < (xNon + wNon) && yMouse >= yNon && yMouse < (yNon + hNon))
		        	{
		        		continuer_echap = 0;
		        	}
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

    SDL_FreeSurface(ecran_tmp);
    SDL_FreeSurface(texte);
    SDL_FreeSurface(menu_echap);
}

bool game_over(game g){
	if(whatGame("rush-hour\n"))
		return game_over_hr(g);
	if(whatGame("klotski\n"))
		return game_over_ar(g);
	printf("game_over(), cas non prévu ...");
	return false;
}

int afficherNbMove(game g){
	return 0;
}

void son_fin(){
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
	{	
		printf("%s", Mix_GetError());
	}	
	Mix_Music *musique; //Création du pointeur de type Mix_Music
	musique = Mix_LoadMUS("Victoire.ogg"); //Chargement de la musique
	Mix_PlayMusic(musique, 0); //Jouer infiniment la musique

	//Pause de quelques secondes, juste pour que le son soit joué
	clock_t arrivee=clock()+(2.5*CLOCKS_PER_SEC); // On calcule le moment où l'attente devra s'arrêter
	while(clock() < arrivee);

	Mix_FreeMusic(musique); //Libération de la musique
	Mix_CloseAudio(); //Fermeture de l'API
}

// Permet d'afficher la fenetre principal, ainsi que le jeu.
void init_sdl_game(game g){
	
	//************************************************** INIT VARIABLE ***********************************************************************
	int NL = game_width(g);//Nombre de case de largeur
	int NH = game_height(g);//Nombre de case hauteur
	int TAILLE_CASE = 75;//taille d'une case (CxC)
	// int CASE = NL * NH;//Nombre de case total
	int WIDTH = NL * TAILLE_CASE + 250;//Largeur de l'écran
	int HEIGHT = NH * TAILLE_CASE + 15;//Hauteur de l'écran

	SDL_Surface *ecran = NULL;//Notre fenetre principale
	// SDL_Surface *grille[NL][NH];//tableau 2D recuperant chaque case

	SDL_Surface ***grille = (SDL_Surface***) malloc(NH * sizeof(SDL_Surface**));
	SDL_Surface **grille2 = (SDL_Surface**)	malloc(NH * NL * sizeof(SDL_Surface*));

	if(grille == NULL)
		error("init_sdl_game(), Probleme d'allocation sur grille");
	if(grille2 == NULL)
		error("init_sdl_game(), Probleme d'allocation sur grille2");
	for(int i = 0; i < NH; i++)
		grille[i] = &grille2[i * NL];

	SDL_Surface *texte = NULL;
	SDL_Surface *sortie_jeu = NULL;
	// SDL_Surface *menu_echap = NULL;
	// SDL_Surface *fond = NULL;
	SDL_Rect position;//Variable position, elle nous permet de positionner les rectangles.
	SDL_Rect position_fin;
	SDL_Event event;//Variable evenement
	
	TTF_Font *police = NULL;

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Color couleurEcriture = {0, 0, 0}; //Couleur du texte -> noir
	SDL_Color couleurFond = {255,255,255};
	SDL_Color couleurBasalt = {77, 83, 84};

	TTF_Init();

	//Pour faire un plein ecran il faut mettre SDL_FULLSCREEN
	ecran = SDL_SetVideoMode(WIDTH - 1, HEIGHT - 1, 32, SDL_HWSURFACE | SDL_DOUBLEBUF); // On définit Ecran
	//On donne un nom a la fenetre et une couleur etc..
	SDL_WM_SetCaption("Puzzle Games", NULL);

	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));

	// **************On donne des valeurs a nos variables ********************
	// fond = IMG_Load("voiture1.jpg");
	police = TTF_OpenFont("Sansation-Regular.ttf", 20);
    texte = TTF_RenderText_Shaded(police, "Puzzle Games", couleurEcriture, couleurFond);

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

	
	//************************************************ INIT SDL  / TTF ************************************************************************
	
	//On crée des rectangles que l'on stock dans le tableau 2D grille
	for(int y = 0; y < NH; y++)
		for(int x = 0; x < NL; x++)
			grille[y][x] = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE_CASE - 1, TAILLE_CASE - 1, 32, 0, 0, 0, 0);

	// *************************************** COLORATION DES CASES *********************************************************************************

	//petits tableaux qui correspondent à des couleurs :) Je pense en mettre une dizaine, et faire un modulo pour gerer les cas avec beaucoup de pieces
	
	afficherGrilleJeu(g, ecran, grille, NL, NH, TAILLE_CASE);

	SDL_FillRect(sortie_jeu, NULL, SDL_MapRGB(ecran->format, 250, 20, 20));
	SDL_BlitSurface(sortie_jeu, NULL, ecran, &position_fin);

	// ****************************************  AFFICHAGE TEXTE  ***********************************************************
	//Nom du jeu global ( Puzzle Games )
	position.x = (((NL * TAILLE_CASE) + WIDTH) / 2) - (texte->w / 2);
    position.y = 6/HEIGHT;
    SDL_BlitSurface(texte, NULL, ecran, &position);
    // SDL_FreeSurface(texte);

    // On affiche le jeu précisement
    char* nomDuJeu = whatGameStr();
    texte = TTF_RenderText_Blended(police, nomDuJeu, couleurEcriture);

    position.x = (((NL * TAILLE_CASE) + WIDTH) / 2) - (texte->w / 2);
    position.y = 6/HEIGHT + 20;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    // ********************************************************* GESTION EVENT ************************************************************************
	int continuer = 1;
	int indice_piece = -1;

	while(continuer && !game_over(g))
	{
		// Grossomodo, il y a deux types de gestions d'Event : SDL_WaitEvent et SDL_PollEvent Le premier evite de manger le processeur car 
		//il met en pause le processus jusqu'a avoir un evenement. Le deuxieme par contre, on lui laisser manger ce qu'il veut, car il va juste attendre
		// qu'un evenement se passe pour faire quelque chose sans mettre le processus en pause. Ainsi, dans le premier cas, on attend a l'instruction 
		//que quelque chose se passe, alors que dans le deuxieme cas, on attend pas a l'instruction, on execute la boucle infini jusqu'a avoir un evenement.
		
		//un ptit lien listant les constantes de la SDL en rapport aux events https://user.oc-static.com/ftp/mateo21/sdlkeysym.html
		SDL_WaitEvent(&event); /* Récupération de l'événement dans event */
	    switch(event.type) /* Test du type d'événement */
	    {
	        case SDL_QUIT: /* Si c'est un événement de type "Quitter" */
	            continuer = 0;
	            break;

	         case SDL_MOUSEBUTTONUP:
		        if (event.button.button == SDL_BUTTON_LEFT)
		            indice_piece = clic(event, g, WIDTH, HEIGHT, NL, NH, TAILLE_CASE);
		        break;
	        
	        case SDL_KEYDOWN:
	        	switch(event.key.keysym.sym)
		        {
		        	case SDLK_ESCAPE:
		        		menu_echap(ecran, &continuer, WIDTH, HEIGHT, couleurFond, couleurBasalt, police);
					    break;
					case SDLK_UP:
						if(indice_piece != -1)
						{
							play_move(g, indice_piece, UP, 1);
							afficherGrilleJeu(g, ecran, grille, NL, NH, TAILLE_CASE);
						}

						break;
					case SDLK_DOWN:
						if(indice_piece != -1)
						{
							play_move(g, indice_piece, DOWN, 1);
							afficherGrilleJeu(g, ecran, grille, NL, NH, TAILLE_CASE);
						}

						break;
					case SDLK_LEFT:
						if(indice_piece != -1)
						{
							play_move(g, indice_piece, LEFT, 1);
							afficherGrilleJeu(g, ecran, grille, NL, NH, TAILLE_CASE);
						}

						break;
					case SDLK_RIGHT:
						if(indice_piece != -1)
						{
							play_move(g, indice_piece, RIGHT, 1);
							afficherGrilleJeu(g, ecran, grille, NL, NH, TAILLE_CASE);
						}

						break;

					default:
						break;
	        	}
	        	break;
	        default:
	        	break;
	    }

	    SDL_Flip(ecran);
	}

	// if(game_over(g))
	// 	son_fin();

	// ******************************************************** fin / FREE DE TOUT ******************************************************************
	//libération des allocations mémoires
	

	TTF_CloseFont(police);
	TTF_Quit();

	// SDL_FreeSurface(grille[0]);
	// SDL_FreeSurface(grille);
	for(int y = 0; y < NH; y++)
		for(int x = 0; x < NL; x++)
			SDL_FreeSurface(grille[y][x]);
	// free(grille[0]);
	// free(grille);
	free(nomDuJeu);
	SDL_FreeSurface(texte);
	SDL_FreeSurface(sortie_jeu);
	
	SDL_Quit();
}

int choixDuJeu(){
	SDL_Surface *ecran = NULL;
	SDL_Surface *texte = NULL;
	SDL_Event event;

	SDL_Rect position;

	TTF_Font *police = NULL;

	int WIDTH = 680;
	int HEIGHT = 420;

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Color couleurEcriture = {0, 0, 0}; //Couleur du texte -> noir
	SDL_Color couleurFond = {255,255,255};
	SDL_Color couleurBasalt = {77, 83, 84};

	TTF_Init();
	
	ecran = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF); 
	
	SDL_WM_SetCaption("Puzzle Games", NULL);
	
	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
	
	police = TTF_OpenFont("Sansation-Regular.ttf", 20);
	texte = TTF_RenderText_Shaded(police, "Puzzle Games - Choix du Jeu", couleurEcriture, couleurFond);

	position.x = (WIDTH - texte->w) / 2;
	position.y = HEIGHT / 8;

	SDL_BlitSurface(texte, NULL, ecran, &position);

	texte = TTF_RenderText_Shaded(police, "Rush Hour", couleurFond, couleurEcriture);

	position.x = (WIDTH - texte->w) / 2;
	position.y = HEIGHT / 3;

	int xRh = position.x;
	int yRh = position.y;
	int wRh = texte->w;
	int hRh = texte->h;

	SDL_BlitSurface(texte, NULL, ecran, &position);

	texte = TTF_RenderText_Shaded(police, "Klotski / L'Ane Rouge", couleurFond, couleurEcriture);

	position.x = (WIDTH - texte->w) / 2;
	position.y = position.y + ( 2 * texte->h );

	int xAr = position.x;
	int yAr = position.y;
	int wAr = texte->w;
	int hAr = texte->h;

	SDL_BlitSurface(texte, NULL, ecran, &position);

	int continuer = 1;
	int xMouse;
	int yMouse;

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
						menu_echap(ecran, &continuer, WIDTH, HEIGHT, couleurFond, couleurBasalt, police);
						if(continuer == 0)
							valeur_retour = -1;
						break;
					default:
						break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				xMouse = event.button.x;
				yMouse = event.button.y;
				if(xMouse >= xRh && xMouse < (xRh + wRh) && yMouse >= yRh && yMouse < (yRh + hRh))
				{
					initFileConfig("rush-hour");
					continuer = 0;
				}
				if(xMouse >= xAr && xMouse < (xAr + wAr) && yMouse >= yAr && yMouse < (yAr + hAr))
				{
					initFileConfig("klotski");
					continuer = 0;
				}
				break;
			default:
				break;
		}

		SDL_Flip(ecran);
	}


	TTF_CloseFont(police);
	TTF_Quit();

	SDL_FreeSurface(texte);

	SDL_Quit();

	return valeur_retour;


}

int main(){

	int retour = choixDuJeu();
	if(retour == -1)
		return EXIT_SUCCESS;

	char *Game1;

	if(whatGame("rush-hour\n"))
		Game1 = "4n6x6p2w2h1x0y3p2w3h1x0y0p1w1h3x2y2p1w1h2x5y2";
	else if(whatGame("klotski\n"))
		Game1 = "10n4x5p3w2h2x2y0p3w1h2x1y0p3w1h2x0y0p3w2h1x0y2p3w2h1x2y2p3w1h2x0y3p3w1h1x1y3p3w1h1x1y4p3w1h1x2y3p3w1h1x2y4";
	
	game g = getGameFromId(Game1);
	init_sdl_game(g);
	delete_game(g);
	

	return EXIT_SUCCESS;

}