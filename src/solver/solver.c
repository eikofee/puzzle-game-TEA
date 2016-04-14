#include <solver.h>
#include <stdio.h>
#include <string.h>



struct map_s{
	struct map_s* from;
	game g;
};

struct nodeQueue_s {
	struct nodeQueue_s* next;
	struct map_s* m;
};

struct list_s{
	struct map_s* m;
	struct list_s* next;
};

typedef struct nodeQueue_s* nodeQueue;
typedef struct map_s* map;
typedef struct list_s* list;

list newListItem(map m, list prev)
{
	list ln = (list) malloc(sizeof(list));
	ln->m = (map) malloc(sizeof(map));
	ln->next = (list) malloc(sizeof(list));
	ln->m = m;
	ln->next = NULL;
	if (prev)
		prev->next = ln;
	return ln;
}

map newMap(game g, map prev)
{
	map m = (map) malloc(sizeof(map));
	m->g = (game) malloc(sizeof(game));
	m->from = (map) malloc(sizeof(map));
	m->g = g;
	m->from = prev;
	return m;
}

nodeQueue newQueueItem(map m, nodeQueue current)
{
	nodeQueue n = (nodeQueue) malloc(sizeof(nodeQueue));
	n->m = (map) malloc(sizeof(map));
	n->next = (nodeQueue) malloc(sizeof(nodeQueue));
	n->m = m;
	n->next = NULL;
	if (current)
		current->next = n;
	return n;	
}
void deleteMap(map m)
{
	free(m->g);
	free(m->from);
	free(m);
}
void deleteQueueItem(nodeQueue n)
{
	free(n->m);
	free(n->next);
	free(n);
}
void deleteListItem(list l)
{
	free(l->m);
	free(l->next);
	free(l);
}
void queueRemove(nodeQueue n)
{	
	//nodeQueue cn = n;
	n = n->next;
	//deleteQueueItem(cn);
}

nodeQueue getTop(nodeQueue n)
{
	while (n->next)
	{
		n = n->next;
	}
	return n;
}

bool isCleared(nodeQueue n)
{
	nodeQueue c = (nodeQueue) malloc(sizeof(nodeQueue));
	c = n;
	return game_over_hr(c->m->g);
}

map createNewState(map m, int nPiece, dir d, int dist)
{
	game g = new_game(1, 1, 0, NULL);
	copy_game(m->g, g);
	map r = NULL;
	if (play_move(g, nPiece, d, dist))
		r = newMap(g, m);
	else
		delete_game(g);
	return r;
}
bool compareMap(map m1, map m2)
{
	char* s1 = (char*) malloc(256*sizeof(char));
	char* s2 = (char*) malloc(256*sizeof(char));
	getIdFromGame(m1->g, s1);
	getIdFromGame(m2->g, s2);
	int r = strcmp(s1, s2);
	free(s1);
	free(s2);
	return (r == 0);	//retourne true si les chaines sont identiques
}
map checkMapExistence(map m, list origin)
{
	if (compareMap(m, origin->m))
	{
		//deleteMap(m);
		//return origin->m;
		return NULL;
	}
	if (origin->next == NULL)
	{
		newListItem(m, origin);
		return m;
	}
	return checkMapExistence(m, origin->next);
}
void fillQueue(nodeQueue currentNode, nodeQueue queueTop, map previousState, list listMap)
{
	//crée une map pour currentNode: currentMap si non présente dans la liste, si elle y est, on récupère

	//cherche tous les coups possibles
	for (int p = 0; p < game_nb_pieces(currentNode->m->g); p++)
		//for (int dist = 1; dist < 5; dist++)
			for (int d = 0; d < 4; d++)
			{
				map r = createNewState(previousState, p, d, 1);

				if (r)
				{
					r = checkMapExistence(r, listMap);
					if (r)
					{
						//drawInterface(r->g, "TEST");
						newQueueItem(r, queueTop);
						queueTop = queueTop->next;
					}
				}
			}
	//pour chaque coup (donc game), on crée une Map: newMap(newGame, currentMap)
	//si la map n'est pas présente dans  listMap, alors on l'y insère. On y ajoute un nouveau node dans la file, à partir de top
		//et on "incrémente" top (top = top->next or something)
}
void trace(nodeQueue final)
{
	map m = (map) malloc(sizeof(map));
	m = final->m;
	while (m->from)
	{
		drawInterface(m->g, "");
		m = m->from;
	}
}
void solve(game g)
{
	
	map origMap = newMap(g, NULL);
	list listMap = newListItem(origMap, NULL);
	nodeQueue root = newQueueItem(origMap, NULL);
	nodeQueue currentNode = root;
	while (!isCleared(currentNode))
	{
		nodeQueue top = getTop(currentNode); 
		fillQueue(currentNode, top, currentNode->m, listMap);
		//queueRemove(currentNode);
		currentNode = currentNode->next;
	}
	printf("Nombre de coup minimal : %d\n", game_nb_moves(currentNode->m->g));
	//on trace la map de currentNode : map->prev jusqu'à NULL, et on a la séquence finale
	//need delete everything else (parcourir les derniers nodes de la pile et effacer les map ?)
	trace(currentNode);
}