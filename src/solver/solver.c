#include <solver.h>
#include <stdio.h>
#include <string.h>

struct node_s {
	char* seq;
	char* single_seq;
	game g;
	struct node_s* parent;
};


struct move_s {
	int numPiece;
	dir d;
	int distance;
};

typedef struct move_s* move;
typedef struct node_s* node;
char* getStringFromMove(move m)
{
	char* s = (char*)malloc(6*sizeof(char));
	s[0] = m->numPiece + '0';
	char d;
	switch(m->d){
		case UP: d = 'u';break;
		case DOWN : d = 'd';break;
		case LEFT : d = 'l';break;
		case RIGHT : d = 'r';break;
		default : d = '?';break;
	}
	s[1] = d;
	s[2] = m->distance + '0';
	s[3] = ';';
	s[4] = '\0';
	return s;
}
move newMove(int numPiece, dir d, int distance)
{
	move m;
	m = (move) malloc(sizeof(struct move_s));
	m->numPiece = numPiece;
	m->d = d;
	m->distance = distance;
	return m;
}

node newNode(char* seq, game g, node parent, char* single_seq)
{
	node n = (node)malloc(sizeof(node));
	n->seq = (char*)malloc(256*sizeof(char));
	n->single_seq = (char*)malloc(6*sizeof(char));
	n->g = new_game(1, 1, 0, NULL);
	strcpy(n->seq, seq);
	strcpy(n->single_seq, single_seq);
	copy_game(g, n->g);
	n->parent = (node)malloc(sizeof(node));
	n->parent = parent;
	return n;
}
node newNodeFromMove(node root, move m)
{
	game g = new_game(1, 1, 0, NULL);
	copy_game(root->g, g);
	play_move(g, m->numPiece, m->d, m->distance);
	char* seq = (char*)malloc(256*sizeof(char));
	strcpy(seq, root->seq);
	strcat(seq, getStringFromMove(m));
	node n = newNode(seq, g, root, getStringFromMove(m));
	return n;
}

bool testMove(game g, int numPiece, dir d, int distance)//, bool* clear)
{
	game tmp;
	tmp = new_game(1,1,0,NULL);
	copy_game(g, tmp);
	bool r = play_move(tmp, numPiece, d, distance);
	//*clear = game_over_hr(tmp);
	delete_game(tmp);
	return r;
}

move* getPossibleMoves(game g, int* len, node n)
{
	int ind = 0;
	move* table = (move*) malloc(12*game_nb_pieces(g)*sizeof(move));
	for (int i = 0; i < game_nb_pieces(g); i++)
	{
		for (int k = 1; k < 6; k++)
			for (int d = 0; d < 4; d++)
			{
				if (testMove(g, i, d, k))//, &n->clear))
				{
					table[ind] = newMove(i, d, k); 
					ind++;
				}
			}
	}
	*len = ind;
	return table;
}

bool prevMet(node n, char* id)
{
	if (n->parent == NULL)
		return false;
	char* idp = (char*) malloc(128*sizeof(char));
	getIdFromGame(n->parent->g, idp);
	return (strcmp(n->single_seq, n->parent->single_seq) == 0 || strcmp(id, idp) == 0 || prevMet(n->parent, id));
}

bool assignChilds(node root, node* childsArray, int* index, node* clearedNode)
{
	char* id = (char*)malloc(128*sizeof(char));
	getIdFromGame(root->g, id);
	if (game_over_hr(root->g))
	{
		clearedNode[0] = root;
		printf("Fini\n");
		return true;
	}
	if (prevMet(root, id))
	{
		return false;
	}
	free(id);
	int nbMoves = 0;
	move* m = getPossibleMoves(root->g, &nbMoves, root);
	for (int i = 0; i < nbMoves; i++)
	{
		childsArray[*index] = newNodeFromMove(root, m[i]);
		printf("Node : %s\n", childsArray[*index]->seq);
		*(index) += 1;
	}
	printf("___\n");
	return false;
}

bool solveArray(node* childsArray, int* len, node* clearedNode)
{
	int index = 0;
	int newLen = game_nb_pieces(childsArray[0]->g)*12*(*len);
	node* newChildsArray = (node*)malloc(newLen*sizeof(node));
	for (int i = 0; i < *len; i++)
	{
		if (assignChilds(childsArray[i], newChildsArray, &index, clearedNode))
			return true;
	}

	return solveArray(newChildsArray, &index, clearedNode);
}

bool solve(game g)
{
	bool r = false;
	int len = 1;
	node* clearedNode = (node*)malloc(sizeof(node));
	node* childsArray = (node*)malloc(sizeof(node));
	childsArray[0] = newNode("", g, NULL, "");
	r = solveArray(childsArray, &len, clearedNode);
	if (r)
		printf("Terminé : %s\n", clearedNode[0]->seq);
	printf("Done\n");
}