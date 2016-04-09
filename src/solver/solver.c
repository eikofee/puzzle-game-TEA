#include <solver.h>
#include <stdio.h>
#include <string.h>
dir revertDirection2(dir d){
	switch(d){
		case UP: return DOWN;
		case DOWN: return UP;
		case LEFT: return RIGHT;
		case RIGHT: return LEFT;
		default: break;
	}
}
struct solverNode_s;
struct move_s {
	int numPiece;
	dir d;
	int distance;
};
typedef struct move_s* move;

struct solverNode_s {
	struct solverNode_s* parent;
	char* value;
	game g;
	int childsNumber;
	bool clear;
	struct solverNode_s** childs;
	move m;
};




typedef struct solverNode_s* solverNode;

move newMove(int numPiece, dir d, int distance)
{
	move m;
	m = (move) malloc(sizeof(struct move_s));
	m->numPiece = numPiece;
	m->d = d;
	m->distance = distance;
	return m;
}
solverNode newNode(solverNode parent, char* value, move m, game g)
{
	solverNode n;
	n = (solverNode)malloc(sizeof(struct solverNode_s));
	n->parent = (solverNode) malloc(sizeof(solverNode));
	n->parent = parent;
	n->value = (char*)malloc(512*sizeof(char));
	//strcpy(n->value, value);
	n->m = (move)malloc(sizeof(move));
	n->m = m;
	n->clear = (bool)malloc(sizeof(bool));
	n->clear = game_over_hr(g);
	n->childsNumber = 0;
	n->childs = (solverNode*)malloc(sizeof(solverNode)*512);
	n->g = new_game(1,1,0,NULL);
	copy_game(g, n->g);
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
move* getPossibleMoves(game g, int* len, solverNode n)
{
	int ind = 0;
	move* table = (move*) malloc(2*game_nb_pieces(g)*sizeof(move));
	for (int i = 0; i < game_nb_pieces(g); i++)
	{
		for (int d = 0; d < 4; d++)
			if (testMove(g, i, d, 1))//, &n->clear))
			{
				table[ind] = newMove(i, d, 1); 
				ind++;
			}
	}
	*len = ind;
	return table;
}
bool compareID(solverNode s, char* id)
{

	if (s->parent == NULL)
		return true;
	bool b = (!strEqual(id, s->value) && compareID(s->parent, id));
	return b;
}
bool uniqueMove(game g, solverNode s, move m)
{
	game tmp;
	tmp = new_game(1,1,0,NULL);
	copy_game(g, tmp);
	play_move(tmp, m->numPiece, m->d, m->distance);
	//Manque le malloc de id (ou pas)
	char* id = (char*)malloc(512*sizeof(char));
	getIdFromGame(tmp, id);
	delete_game(tmp);
	if (compareID(s, id))
		return true;
	return false;
}
bool assignChilds(game g, solverNode n)
{
	int childsNumber = 0;
	move* moveList = getPossibleMoves(g, &childsNumber, n);
	solverNode* childs = (solverNode*)malloc(childsNumber*sizeof(solverNode));
	int j = 0;
	for (int i = 0; i < childsNumber; i++)
	{
		//if (uniqueMove(g, n, moveList[i]))
		//{
			solverNode s = newNode(n, "", moveList[i], n->g);
			childs[j] = s;
			j++;
		//}
	}
	n->childs = childs;
	n->childsNumber = j;
	if (childsNumber == 0)
		printf("Aucun enfant, fin d'un noeud\n");
	//get the moves
	//test if already known
	//create the childs
	//assign
	//get deeper
	return !j;
}
bool createTree(game g, solverNode root, solverNode* clearArray, int* caInd)
{
	if (root->clear)
	{
		clearArray[*caInd] = root;
		*caInd += 1;
		return true;
	}
	char* id = (char*)malloc(512*sizeof(char));
	play_move(root->g, root->m->numPiece, root->m->d, root->m->distance);
	getIdFromGame(g, id);
	strcpy(root->value, id);
	if (!game_over_hr(root->g) && compareID(root, root->value))
		assignChilds(root->g, root);
	else
		printf("Similitude trouvée\n");
	for (int i = 0; i < root->childsNumber; i++)
		createTree(root->g, root->childs[i], clearArray, caInd);
	printf("Fin d'un noeud\n");
	return true;
	
	//set childs n stuff
}
int getShorterPath(solverNode* table, int size)
{
	solverNode n;
	int minInd = 0;
	int minLen = -1;
	for (int i = 0; i < size; i++)
	{
		int len = 0;
		n = table[i];
		while(n->parent != NULL)
		{
			len++;
			n = n->parent;
		}
		if (len < minLen || minLen == -1)
		{
			minInd = i;
			minLen = len;
		}
	}
	return minInd;
}
void displayShorterPath(solverNode* table, int ind) //AFFICHE EN SENS INVERSE
{
	solverNode n;
	n = table[ind];
	while (n != NULL)
	{
		move m = n->m;
		char c;
		switch (m->d)
		{
			case UP:c = 'u';break;
			case DOWN:c = 'd';break;
			case LEFT:c = 'l';break;
			case RIGHT:c = 'r';break;
			default: c='?';break;
		}
		printf("%d%c%d", m->numPiece, c, m->d);
		n = n->parent;
	}
}
bool solve(game g, char* id)
{
	solverNode* masterTable;
	masterTable = (solverNode*)malloc(2048*sizeof(solverNode));
	int caInd = 0;
	move m = newMove(0, 0, 0);
	solverNode root = newNode(NULL, id, m, g);
	//strcpy(root->value, id);
	createTree(g, root, masterTable, &caInd);
	//Fin graph
	printf("Fin de la recherche, solutions trouvées : %d\n", caInd);
	int bestInd = getShorterPath(masterTable, caInd);
	displayShorterPath(masterTable, bestInd);
}