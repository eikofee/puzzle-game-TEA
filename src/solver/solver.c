#include <solver.h>
#include <stdio.h>
//#include <string.h>
//#include <time.h>

//Remplacer "//SR" par du vide pour rendre le tra�age fonctionnel

struct map_s{
	//SRstruct map_s* from;
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

map newMap(game g)//SR, map prev)
{
	map m = (map) malloc(sizeof(struct map_s));
	m->g = new_game(1,1,0,NULL);
	//SRm->from = prev;
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
	delete_game(n->m->g);
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

bool isCleared(nodeQueue n, bool isRH)
{
	// nodeQueue c = (nodeQueue) malloc(sizeof(struct nodeQueue_s));
	// c = n;
	bool cleared;
	if (isRH)
		cleared = game_over_hr(n->m->g);
	else
		cleared = game_over_ar(n->m->g);
	return cleared;
}

map createNewState(map m, int nPiece, dir d, int dist)
{
	game g = new_game(1, 1, 0, NULL);
	copy_game(m->g, g);
	map r = NULL;
	if (play_move(g, nPiece, d, dist)){
		r = newMap(g);//SR, m);
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
		delete_game(m->g);
		free(m);
		return NULL;
	}
	if (origin->next == NULL)
	{
		newListItem(m, origin);
		return m;
	}
	return checkMapExistence(m, origin->next);
}
void fillQueue(nodeQueue currentNode, nodeQueue queueTop, map previousState, list listMap, bool* cleared, bool isRH)
{
	//crée une map pour currentNode: currentMap si non présente dans la liste, si elle y est, on récupère
	//cherche tous les coups possibles
	for (int p = 0; p < game_nb_pieces(currentNode->m->g); p++)
	{
		//for (int dist = 1; dist < 1; dist++)
			for (int d = 0; d < 4; d++)
			{

				map r = createNewState(previousState, p, d, 1);
				if (r != NULL)
				{
					r = checkMapExistence(r, listMap);
					if (r != NULL)
					{
						//drawInterface(r->g, "TEST");
						queueTop = newQueueItem(r, queueTop);
						if (isCleared(queueTop, isRH))
						{
							//drawInterface(queueTop->m->g, "TEST");
							currentNode = queueTop;
							*cleared = true;
							return;
						}
						// queueTop = queueTop->next;
					}
				}
			}
	}
	
	//pour chaque coup (donc game), on crée une Map: newMap(newGame, currentMap)
	//si la map n'est pas présente dans  listMap, alors on l'y insère. On y ajoute un nouveau node dans la file, �? partir de top
		//et on "incrémente" top (top = top->next or something)
}

//SRvoid trace(nodeQueue final)
//SR{
	// map m = final-> m;
	// m = final->m;
	
//SR	while (final->m ->from)
//SR	{
//SR		map tmp = final->m;
//SR		drawInterface(final->m->g, "");
//SR		delete_game(final->m->g);
//SR		final->m = final->m->from;
//SR		free(tmp);
//SR	}
//SR}
void solve(game g, bool rh)
{
	//bool rh = whatGame("rush-hour\n");
	//clock_t c1;
	//clock_t c2;
	//float temps;
	//c1 = clock();
	map origMap = newMap(g);//SR, NULL);
	list listMap = newListItem(origMap, NULL);
	nodeQueue root = newQueueItem(origMap, NULL);
	nodeQueue currentNode = root;
	nodeQueue top = root;
	bool cleared = false;
	while (!cleared && currentNode)
	{
		top = getTop(top);
		fillQueue(currentNode, top, currentNode->m, listMap, &cleared, rh);
		//queueRemove(currentNode);
		if (!cleared){
			currentNode = currentNode->next;
		}
	}
	//c2 = clock();
	//temps = (float)(c2-c1) / CLOCKS_PER_SEC;
	//drawInterface(currentNode->m->g, "TEST");
	//drawInterface(top->next->m->g, "TEST");
	//printf("Nombre de coup minimal : %d, temps de calcul : %fs\n", game_nb_moves(currentNode->m->g), temps);
	
	//drawInterface(top->m->g, "TEST");
	//printf("Nombre de coup minimal : %d, temps de calcul : %fs\n", game_nb_moves(top->m->g), temps);
	int nbFinal = cleared?game_nb_moves(top->next->m->g):-1;
	printf("%d\n", nbFinal);
	//on trace la map de currentNode : map->prev jusqu'�? NULL, et on a la séquence finale
	//need delete everything else (parcourir les derniers nodes de la pile et effacer les map ?)
	//trace(currentNode->next);
	//nodeQueue tmp_node = currentNode;
	nodeQueue tmp_node = root;
	while(tmp_node != NULL){
		currentNode = tmp_node;
		tmp_node = currentNode -> next;
		if(currentNode -> m -> g != NULL){
			delete_game(currentNode -> m -> g);
			if (currentNode -> m != NULL)
				free(currentNode -> m);
		}
		free(currentNode);
	}
	// free(root);
	// delete_game(root -> m -> g);
	// free(root -> m);
	//if (!(nbFinal == game_nb_moves(g))){
	//	delete_game(root -> m -> g);
	//	free(root -> m);
	//	free(root);
	//}
	list tmp_list = listMap;
	while(tmp_list != NULL){
		listMap = tmp_list;
		tmp_list= listMap -> next;
		free(listMap);
	}
}
