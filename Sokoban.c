#include <stdio.h>
#include <malloc.h>
#include <math.h>

#define Maxlength 10000

#define num_rows 9
#define num_cols 8
#define way 0
#define wall 1
#define box 2
#define current 3
#define boxincurrent 5
#define man 6
#define manincurrent 9

typedef struct{
	int map[num_rows][num_cols];
	int man_row, man_col;
}State;

typedef struct Node{
	State state;
	struct Node* parent;
	int no_function;
	int h;
}Node;

int moveUp(State state, State *result){
	*result = state;
	result->man_row = state.man_row - 1;
	result->man_col = state.man_col;
	result->map[state.man_row][state.man_col] -= man;
	result->map[state.man_row - 1][state.man_col] += man;

	if(state.map[state.man_row - 1][state.man_col] == way || state.map[state.man_row - 1][state.man_col] == current){
		return 1;
	}else if(state.map[state.man_row - 1][state.man_col] == box || state.map[state.man_row - 1][state.man_col] == boxincurrent){
		int box_row = state.man_row - 1;
		int box_col = state.man_col;
		if(state.map[box_row - 1][box_col] == way || state.map[box_row - 1][box_col] == current){
			result->map[box_row - 1][box_col] += box;
			result->map[box_row][box_col] -= box;
			return 1;
		}
	}
	
	return 0;
}

int moveDown(State state, State *result){
	*result = state;
	result->man_row = state.man_row + 1;
	result->man_col = state.man_col;
	result->map[state.man_row][state.man_col] -= man;
	result->map[state.man_row + 1][state.man_col] += man;
	
	if(state.map[state.man_row + 1][state.man_col] == way || state.map[state.man_row + 1][state.man_col] == current){
		return 1;
	}else if(state.map[state.man_row + 1][state.man_col] == box || state.map[state.man_row + 1][state.man_col] == boxincurrent){
		int box_row = state.man_row + 1;
		int box_col = state.man_col;
		if(state.map[box_row + 1][box_col] == way || state.map[box_row + 1][box_col] == current){
			result->map[box_row + 1][box_col] += box;
			result->map[box_row][box_col] -= box;
			return 1;
		}
	}
	
	return 0;
}

int moveLeft(State state, State *result){
	*result = state;
	result->man_row = state.man_row;
	result->man_col = state.man_col - 1;
	result->map[state.man_row][state.man_col] -= man;
	result->map[state.man_row][state.man_col - 1] += man;
	
	if(state.map[state.man_row][state.man_col - 1] == way || state.map[state.man_row][state.man_col - 1] == current){
		return 1;
	}else if(state.map[state.man_row][state.man_col - 1] == box || state.map[state.man_row][state.man_col - 1] == boxincurrent){
		int box_row = state.man_row;
		int box_col = state.man_col - 1;
		if(state.map[box_row][box_col - 1] == way || state.map[box_row][box_col - 1] == current){
			result->map[box_row][box_col - 1] += box;
			result->map[box_row][box_col] -= box;
			return 1;
		}
	}
	
	return 0;
}

int moveRight(State state, State *result){
	*result = state;
	result->man_row = state.man_row;
	result->man_col = state.man_col + 1;
	result->map[state.man_row][state.man_col] -= man;
	result->map[state.man_row][state.man_col + 1] += man;
	
	if(state.map[state.man_row][state.man_col + 1] == way || state.map[state.man_row][state.man_col + 1] == current){
		return 1;
	}else if(state.map[state.man_row][state.man_col + 1] == box || state.map[state.man_row][state.man_col + 1] == boxincurrent){
		int box_row = state.man_row;
		int box_col = state.man_col + 1;
		if(state.map[box_row][box_col + 1] == way || state.map[box_row][box_col + 1] == current){
			result->map[box_row][box_col + 1] += box;
			result->map[box_row][box_col] -= box;
			return 1;
		}
	}
	
	return 0;
}

int callOperator(State state, State *result, int opt){
	switch(opt){
		case 1: return moveUp(state, result);
		case 2: return moveDown(state, result);
		case 3: return moveLeft(state, result);
		case 4: return moveRight(state, result);
		default: printf("Error call operator!");
	}
}

//void printState(State state){
//	int i, j;
//	for(i = 0; i < num_rows; i++){
//		for(j = 0; j < num_cols; j++){
//			switch(state.map[i][j]){
//				case wall:
//					printf("#");
//					break;
//				case box:
//					printf("x");
//					break;
//				case current:
//					printf("-");
//					break;
//				case boxincurrent:
//					printf("o");
//					break;
//				case man: 
//				case manincurrent:
//					printf("$");
//					break;
//				default:
//					printf(" ");
//			}
//		}
//		printf("\n");
//	}
//}

int compareState(State state1, State state2){
	int i, j;
	for(i = 0; i < num_rows; i++){
		for(j = 0; j < num_cols; j++){
			if(state1.map[i][j] != state2.map[i][j])
				return 0;
		}
	}
	
	return 1;
}

int checkGoal(State state){
	int i, j;
	for(i = 0; i < num_rows; i++){
		for(j = 0; j < num_cols; j++){
			if(state.map[i][j] == box || state.map[i][j] == current)
				return 0;		
		}
	}
	
	return 1;
}

typedef struct{
	Node* Elements[Maxlength];
	int size;
}List;

void makeNull_List(List *list){
	list->size = 0;
}

int empty_List(List list){
	return list.size == 0;	
}

int full_List(List list){
	return list.size == Maxlength;
}

Node* element_at(List list, int p){
	return list.Elements[p - 1];
}

void push_List(List *list, Node* x, int p){
	if(!full_List(*list)){
		int i;
		for(i = list->size; i >= p; i--){
			list->Elements[i] = list->Elements[i - 1];
		}
		list->Elements[p - 1] = x;
		list->size++;
	}else{
		printf("List is full\n");
	}
}

void delete_List(List *list, int p){
	if(!empty_List(*list)){
		int i;
		for(i = p - 1; i < list->size; i++){
			list->Elements[i] = list->Elements[i + 1];
		}
		list->size--;
	}else{
		printf("List is empty!");
	}
}

void sort_List(List *list){
	int i, j;
	for(i = 1; i < list->size; i++)
		for(j = i + 1; j <= list->size; j++)
			if(element_at(*list, i)->h > element_at(*list, j)->h){
				Node* temp = element_at(*list, i);
				list->Elements[i - 1] = element_at(*list, j);
				list->Elements[j - 1] = temp;
			}
}

int findState(State state, List list, int *p){
	int i;
	for(i = 1; i <= list.size; i++){
		if(compareState(state, element_at(list, i)->state)){
			*p = i;
			return 1;
		}
	}
	return 0;
}

typedef struct{
	int u, v, g;
}Edge;

typedef struct{
	Edge Elements[Maxlength];
	int size;
}EdgeList;

void makeNull_EdgeList(EdgeList *list){
	list->size = 0;
}

int empty_EdgeList(EdgeList list){
	return list.size == 0;	
}

int full_EdgeList(EdgeList list){
	return list.size == Maxlength;
}

Edge element_at_EdgeList(EdgeList list, int p){
	return list.Elements[p - 1];
}

void push_EdgeList(EdgeList *list, Edge x, int p){
	if(!full_EdgeList(*list)){
		int i;
		for(i = list->size; i >= p; i--){
			list->Elements[i] = list->Elements[i - 1];
		}
		list->Elements[p - 1] = x;
		list->size++;
	}else{
		printf("EdgeList is full\n");
	}
}

void delete_EdgeList(EdgeList *list, int p){
	if(!empty_EdgeList(*list)){
		int i;
		for(i = p - 1; i < list->size; i++){
			list->Elements[i] = list->Elements[i + 1];
		}
		list->size--;
	}else{
		printf("EdgeList is empty!");
	}
}

void sort_EdgeList(EdgeList *list){
	int i, j;
	for(i = 1; i < list->size; i++)
		for(j = i + 1; j <= list->size; j++)
			if(element_at_EdgeList(*list, i).g > element_at_EdgeList(*list, j).g){
				Edge temp = element_at_EdgeList(*list, i);
				list->Elements[i - 1] = element_at_EdgeList(*list, j);
				list->Elements[j - 1] = temp;
			}
}

typedef struct{
	int Elements[Maxlength];
	int size;
}VertexList;

void makeNull_VertexList(VertexList *list){
	list->size = 0;
}

int full_VertexList(VertexList list){
	return list.size == Maxlength;
}

int element_at_VertexList(VertexList list, int p){
	return list.Elements[p - 1];
}

void push_VertexList(VertexList *list, int x, int p){
	if(!full_VertexList(*list)){
		int i;
		for(i = list->size; i >= p; i--){
			list->Elements[i] = list->Elements[i - 1];
		}
		list->Elements[p - 1] = x;
		list->size++;
	}else{
		printf("VertexList is full\n");
	}
}

int checkExistVertexList(VertexList list, int vertex){
	int i;
	for(i = 1; i <= list.size; i++){
		if(element_at_VertexList(list, i) == vertex)
			return 1;
	}
	
	return 0;
}

int heuristic(State state){
	int h = 0;
	EdgeList list;
	makeNull_EdgeList(&list);
	
	int u = 0, v = 0;
	int i, j, l, k;
	for(i = 0; i < num_rows; i++){
		for(j = 0; j < num_cols; j++){
			if(state.map[i][j] == box){
				for(l = 0; l < num_rows; l++){
					for(k = 0; k < num_cols; k++){
						if(state.map[l][k] == current){
							Edge edge;
							edge.u = u;
							edge.v = v;
							edge.g = abs(l - i) + abs(k - j);
							push_EdgeList(&list, edge, list.size + 1);
							v++;
						}	
					}
				}
				u++;
				v = 0;
			}	
		}
	}
	
	sort_EdgeList(&list);
	
	VertexList matchedBox, matchedCurrent;
	makeNull_VertexList(&matchedBox);
	makeNull_VertexList(&matchedCurrent);
	
	while(!empty_EdgeList(list)){
		Edge edge = element_at_EdgeList(list, 1);
		delete_EdgeList(&list, 1);
		if(!checkExistVertexList(matchedBox, edge.u) && !checkExistVertexList(matchedCurrent, edge.v)){
			h += edge.g;
			push_VertexList(&matchedBox, edge.u, matchedBox.size + 1);
			push_VertexList(&matchedCurrent, edge.v, matchedCurrent.size + 1);
		}
	}
	

	return h;
}

int isDeadlock(State state){
	int i, j;
	for(i = 0; i < num_rows; i++){
		for(j = 0; j < num_cols; j++){
			if(state.map[i][j] == box){
				if((state.map[i - 1][j] == wall || state.map[i + 1][j] == wall) && (state.map[i][j - 1] == wall || state.map[i][j + 1] == wall)){
					return 1;
				}
			}	
		}
	}
	
	return 0;
}

Node* BestFirstSearch(State start){
	List open, close;
	makeNull_List(&open);
	makeNull_List(&close);
	Node* root = (Node*)malloc(sizeof(Node));
	root->state = start;
	root->parent = NULL;
	root->no_function = 0;
	root->h = heuristic(start);
	push_List(&open, root, 1);
	
	while(!empty_List(open)){
		Node* node = element_at(open, 1);
		delete_List(&open, 1);
		push_List(&close, node, close.size + 1);
		
		if(checkGoal(node->state))
			return node;
		
		int i;
		for(i = 1; i <= 4; i++){
			State result;
			if(callOperator(node->state, &result, i)){
				if(!isDeadlock(result)){
					Node* newNode = (Node*)malloc(sizeof(Node));
					newNode->state = result;
					newNode->parent = node;
					newNode->no_function = i;
					newNode->h = heuristic(result);
					
					int p;
					if(!findState(result, open, &p) && !findState(result, close, &p)){
						push_List(&open, newNode, open.size + 1);
					}else if(findState(result, open, &p) && newNode->h < element_at(open, p)->h){
						delete_List(&open, p);
						push_List(&open, newNode, open.size + 1);
					}else if(findState(result, close, &p) && newNode->h < element_at(close, p)->h){
						delete_List(&close, p);
						push_List(&open, newNode, open.size + 1);
					}
					
					sort_List(&open);
				}
			}
		}
	}
	
	return NULL;
}

void printWayToGoal(Node* node){
	List list;
	makeNull_List(&list);
	
	while(node != NULL){
		push_List(&list, node, list.size + 1);
		node = node->parent;
	}
	
	FILE *f = fopen("E:\\HocTap\\TriTueNhanTao\\baitapnhom\\result.txt", "w");
	int i, l, k;
	for(i = list.size; i > 0; i--){
		for(l = 0; l < num_rows; l++){
			for(k = 0; k < num_cols; k++){
				fprintf(f, "%d ", element_at(list, i)->state.map[l][k]);
			}
			fprintf(f, "\n");
		}
	}
	
	fclose(f);
}

int main(){
	State start;
	
	FILE *f = fopen("E:\\HocTap\\TriTueNhanTao\\baitapnhom\\start.txt", "r");
	fscanf(f, "%d%d", &start.man_row, &start.man_col);
	int i, j;
	for(i = 0; i < num_rows; i++){
		for(j = 0; j < num_cols; j++){
			fscanf(f, "%d", &start.map[i][j]);
		}
	}
	
	Node* node = BestFirstSearch(start);
	printWayToGoal(node);
	
	fclose(f);
	return 0;
}
