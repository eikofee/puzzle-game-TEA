#include <solver.h>
#include <stdio.h>
#include <string.h>
#include <time.h>



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
	list ln = (list) malloc(sizeof(struct list_s));
	ln->m = m;
	ln->next = NULL;
	if (prev)
		prev->next = ln;
	return ln;
}

map newMap(game g, map prev)
{
	map m = (map) malloc(sizeof(struct map_s));
	m->g = new_game(1,1,0,NULL);
	m->from = prev;
	copy_game(g,m->g);
	return m;
}

nodeQueue newQueueItem(map m, nodeQueue current)
{
	nodeQueue n = (nodeQueue) malloc(sizeof(struct nodeQueue_s));
	//n->m = (map) malloc(sizeof(struct map_s));
	n->next = NULL;
	n->m = m;
	if (current)
		current->next = n;
	return n;	
}
// void deleteMap(map m)
// {
// 	delete_game(m->g);
// 	free(m);
// }
void deleteQueueItem(nodeQueue n)
{
	free(n->m);
	free(n->next);
	free(n);
}
// void deleteListItem(list l)
// {
// 	//free(l -> m);
// 	free(l->next);
// 	free(l);
// }
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
	// nodeQueue c = (nodeQueue) malloc(sizeof(struct nodeQueue_s));
	// c = n;
	bool cleared = game_over_hr(n->m->g);
	return cleared;
}

map createNewState(map m, int nPiece, dir d, int dist)
{
	game g = new_game(1, 1, 0, NULL);
	copy_game(m->g, g);
	map r = NULL;
	if (play_move(g, nPiece, d, dist)){
		r = newMap(g, m);
		delete_game(g);
	}
	else
		delete_game(g);
	return r;
}
bool compareMap(map m1, map m2)
{
	//char* s1 = (char*) malloc(256*sizeof(char));
	//char* s2 = (char*) malloc(256*sizeof(char));
	//getIdFromGame(m1->g, s1);
	//getIdFromGame(m2->g, s2);
	//int r = strcmp(s1, s2);
	//free(s1);
	//free(s2);
	for (int i = 0; i < game_nb_pieces(m1->g); i++)
	{
		if ((get_x(game_piece(m1->g, i)) != get_x(game_piece(m2->g, i))) || (get_y(game_piece(m1->g, i)) != get_y(game_piece(m2->g, i))))
			return false;			
	}
	return true;
	//return (r == 0);	//retourne true si les chaines sont identiques
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
void fillQueue(nodeQueue currentNode, nodeQueue queueTop, map previousState, list listMap, bool* cleared)
{
	//crée une map pour currentNode: currentMap si non présente dans la liste, si elle y est, on récupère

	//cherche tous les coups possibles
	for (int p = 0; p < game_nb_pieces(currentNode->m->g); p++)
	{
		//for (int dist = 1; dist < 1; dist++)
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
						if (isCleared(queueTop))
						{
							currentNode = queueTop;
							*cleared = true;
							return;
						}
						queueTop = queueTop->next;
					}
				}
			}
	}
	
	//pour chaque coup (donc game), on crée une Map: newMap(newGame, currentMap)
	//si la map n'est pas présente dans  listMap, alors on l'y insère. On y ajoute un nouveau node dans la file, �? partir de top
		//et on "incrémente" top (top = top->next or something)
}

void trace(nodeQueue final)
{
	// map m = final-> m;
	// m = final->m;
	
	while (final->m ->from)
	{
		map tmp = final->m;
		drawInterface(final->m->g, "");
		delete_game(final->m->g);
		final->m = final->m->from;
		free(tmp);
	}
}
void solve(game g)
{
	clock_t c1;
	clock_t c2;
	float temps;
	c1 = clock();
	map origMap = newMap(g, NULL);
	list listMap = newListItem(origMap, NULL);
	nodeQueue root = newQueueItem(origMap, NULL);
	nodeQueue currentNode = root;
	nodeQueue top = root;
	bool cleared = false;
	while (!cleared)
	{
		top = getTop(top);
		fillQueue(currentNode, top, currentNode->m, listMap, &cleared);
		//queueRemove(currentNode);
		currentNode = currentNode->next;
	}
	c2 = clock();
	temps = (float)(c2-c1) / CLOCKS_PER_SEC;
	printf("Nombre de coup minimal : %d, temps de calcul : %fs\n", game_nb_moves(currentNode->m->g), temps);
	//on trace la map de currentNode : map->prev jusqu'�? NULL, et on a la séquence finale
	//need delete everything else (parcourir les derniers nodes de la pile et effacer les map ?)
	trace(top);
	delete_game(origMap -> g);
	nodeQueue tmp_node = currentNode;
	while(tmp_node != NULL){
		currentNode = tmp_node;
		tmp_node = currentNode -> next;
		free(currentNode);
	}
	deleteQueueItem(root);
	list tmp_list = listMap;
	while(tmp_list != NULL){
		listMap = tmp_list;
		tmp_list= listMap -> next;
		free(listMap);
	}
}