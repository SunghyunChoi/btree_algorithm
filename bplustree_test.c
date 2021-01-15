#include<stdio.h>
#include<malloc.h>
#include<stdbool.h>
#define TEST { 10,1,3,63,82,6,31,8,2,16,11,77,96,14,92,21,47,23,24,78,26,97,15,4,30,69,37,38,76,90,17,87,53,44,45,46,9,41,54,43,22,84,58,39,65,28,42,59,99,70,71,72,29,74,73,68,13,60,79,80,81,85,83,64,94,86,66,88,93,40,91,62,25,20,36,95,19,52,55,100 }

// 확인 함수
void print_for_exam(struct Node* cur);

int order = 4;
int t=2;

//구조체 선언
typedef struct btNode {
	bool isLeaf;
	int n;
	int* key;
	int* data;
	struct btNode** cp;
	struct btNode* link; 
}btNode;
typedef struct bTree {
	struct btNode* root;
	struct btNode* head;
}bTree;
typedef struct find_siblings_result {
	int direction;
	bool result;
}fs;
typedef struct search_result{
	int key;
	int data;
}sr;
//초기화
btNode* createNode(bool isleaf) {
	btNode* node = (btNode*)malloc(sizeof(btNode));
	node->isLeaf = isleaf;
	node->link = NULL;
	node->n = 0;
	node->key = (int*)malloc(sizeof(int) * (order - 1));
	node->data = (int*)malloc(sizeof(int) * (order - 1));
	node->cp = (btNode**)malloc(sizeof(btNode*) * (order));
	node->link = (btNode*)malloc(sizeof(btNode));
	return node;
}
bTree* createTree() {
	bTree* tree = (bTree*)malloc(sizeof(bTree));
	btNode* r = createNode(true);
	tree->root = r;
	tree->head = r;
	return tree;
}

void print_all(btNode* x, int depth);
//삽입
bool splitChild(btNode* x, int i) {
	btNode* y = x->cp[i];
	btNode* z = createNode(y->isLeaf);
	
	if (y->isLeaf == true) {
		for (int j = x->n - 1; j >= i; j--) {
			x->key[j + 1] = x->key[j];
			x->cp[j + 2] = x->cp[j + 1];
		}
		x->n += 1;
		x->cp[i + 1] = z;
		x->key[i] = y->key[t-1];
		z->key[0] = y->key[t];
		z->data[0] = y->data[t];
		for (int j = t; j < y->n; j++) {
			z->key[j - t + 1] = y->key[j];
			z->data[j - t + 1] = y->data[j];
		}
		y->n = t;
		z->n = t-1;
		z->link = y->link;
		y->link = z;
	}
	else {
		//x의 key와 cp를 한칸씩 미룬다(공간 확보)
		for (int j = x->n - 1; j >= i; j--) {
			x->key[j + 1] = x->key[j];
			x->cp[j + 2] = x->cp[j + 1];
		}
		x->n += 1;
		x->cp[i + 1] = z;
		x->key[i] = y->key[t - 1];

		for (int j = t; j < y->n; j++) {
			z->key[j - t] = y->key[j];
			z->cp[j - t] = y->cp[j];
		}
		z->cp[t-1] = y->cp[y->n];
		y->n = t - 1;
		z->n = t - 1;
	}

}
bool insertNonfull(btNode* x, int k, int d) {
	int i = x->n;
	if (x->isLeaf == true) {
		while (i > 0 && k < x->key[i - 1]) {
			x->key[i] = x->key[i - 1];
			x->data[i] = x->data[i - 1];
			i -= 1;
		}
		x->key[i] = k;
		x->data[i] = d;
		x->n += 1;
	}
	else {
		while (i > 0 && k < x->key[i - 1]) {
			i -= 1;
		}
		if (x->cp[i]->n == 2*t - 1) {
			splitChild(x, i);
			if (k > x->key[i]) {
				i += 1;
			}
		}
		insertNonfull(x->cp[i], k, d);
	}
}
bool insertNode(bTree* tree, int k, int d) {
	btNode* r = tree->root;
	if (r->n == 2 * t - 1) {
		btNode* s = createNode(false);
		tree->root = s;
		s->cp[0] = r;
		splitChild(s, 0);
		insertNonfull(s, k, d);
	}
	else {
		insertNonfull(r, k, d);
	}
}

//검색
fs* find_siblings(btNode* x, int i) {
	
	if (i == x->n) {
		fs* find_result = (fs*)malloc(sizeof(fs));
		find_result->direction = -1;
		if (x->cp[i - 1]->n == t - 1) {
			find_result->result = true;
			return find_result;
		}
		else {
			find_result->result = false;
			return find_result;
		}
	}
	else {
		fs* find_result = (fs*)malloc(sizeof(fs));
		find_result->direction = 1;
		if (x->cp[i + 1]->n == t - 1) {
			find_result->result = true;
			return find_result;
		}
		else {
			find_result->result = false;
			return find_result;
		}
	}
}

//병합
bool mergeRightNode(btNode* x, int i) {
	//i번째 노드와 i+1번째 노드를 합친다.
	btNode* y = x->cp[i];
	btNode* z = x->cp[i+1];
	if (y->isLeaf == true) {
		for (int j = i + 1; j < x->n; j++) {
			x->cp[j] = x->cp[j + 1];
		}
		for (int j = i; j < x->n - 1; j++) {
			x->key[j] = x->key[j + 1];
		}
		for (int j = 0; j < t-1; j++) {
			y->key[j + t - 1] = z->key[j];
			y->data[j + t - 1] = z->data[j];
		}
		y->n = 2*t - 2;
		y->link = z->link;
		x->n -= 1;
		free(z->cp);
		free(z->key);
		free(z->data);
		free(z);
	}
	else {
		y->key[t - 1] = x->key[i];
		for (int j = i+1; j < x->n; j++) {
			x->cp[j] = x->cp[j + 1];
		}
		for (int j = i; j < x->n - 1; j++) {
			x->key[j] = x->key[j + 1];
		}
		
		y->cp[t] = z->cp[0];
		for (int j = 0; j < z->n; j++) {
			y->key[j + t] = z->key[j];
			y->cp[j + t+1] = z->cp[j+1];
		}
		y->n = 2 * t - 1;
		x->n -= 1;
		free(z->cp);
		free(z->key);
		free(z->data);
		free(z);
	}
}

//삭제
void deleteNode(btNode* x, int k, bTree* tree) {
	int i;
	if (x->isLeaf == true) {
		for (int i = 0; i < x->n; i++) {
			if (k == x->key[i]) {
				for (int j = i; j <x->n-1; j++) {
					x->key[j] = x->key[j + 1];
					x->data[j] = x->data[j + 1];
				}
				x->n -= 1; 
				return;
			}
		}
	}
	else {
		i = x->n;
		while (k <= x->key[i - 1] && i>0) {
			i -= 1;
		}
		//다음 갈 경로 정했음
		if (x->cp[i]->n >= t) {
			deleteNode(x->cp[i], k, tree);
		}
		else {
			fs* find_result = find_siblings(x, i);
			if ((find_result->direction == 1) && (find_result->result == true)) {				
				mergeRightNode(x, i);
				if (tree->root->n == 0) {
					tree->root = x->cp[i];
					free(x->cp);
					free(x->key);
					free(x->data);
					free(x);
					deleteNode(tree->root, k, tree);
				}
				else{
					deleteNode(x->cp[i], k, tree);
				}
			}
			else if ((find_result->direction == 1) && (find_result->result == false)) {
				btNode* y = x->cp[i];
				btNode* z = x->cp[i + 1];
				if (y->isLeaf == true) {
					x->key[i] = z->key[0];
					y->key[y->n] = z->key[0];
					y->data[y->n] = z->data[0];
					for (int j = 0; j < z->n - 1; j++) {
						z->key[j] = z->key[j + 1];
						z->data[j] = z->data[j + 1];
					}
					y->n += 1;
					z->n -= 1;
					deleteNode(y, k, tree);
				}
				else {
					y->key[y->n] = x->key[i];
					y->cp[y->n+1] = z->cp[0];
					x->key[i] = z->key[0];
					for (int j = 0; j < z->n - 1; j++) {
						z->key[j] = z->key[j + 1];
						z->cp[j] = z->cp[j + 1];
					}
					z->cp[z->n - 1] = z->cp[z->n];
					y->n += 1;
					z->n -= 1;
					deleteNode(y, k, tree);
				}
			}
			else if ((find_result->direction == -1) && (find_result->result == true)) {
				mergeRightNode(x, i-1);
				if (tree->root->n == 0) {
					tree->root = x->cp[i-1];
					free(x->cp);
					free(x->key);
					free(x->data);
					free(x);
					deleteNode(tree->root, k, tree);
				}
				else {
					deleteNode(x->cp[i - 1], k, tree);
				}
			}
			else if ((find_result->direction == -1) && (find_result->result == false)) {
				btNode* y = x->cp[i - 1];
				btNode* z = x->cp[i];
				if (y->isLeaf == true) {
					x->key[i-1] = y->key[y->n - 2];
					for (int j = z->n; j >0; j--) {
						z->key[j] = z->key[j - 1];
						z->data[j] = z->data[j - 1];
					}
					z->key[0] = y->key[y->n - 1];
					z->data[0] = y->data[y->n - 1];
					y->n -= 1;
					z->n += 1;
					deleteNode(z, k, tree);
				}
				else {
					z->cp[z->n + 1] = z->cp[z->n];
					for (int j = z->n; j > 0; j--) {
						z->key[j] = z->key[j-1];
						z->cp[j] = z->cp[j - 1];
					}
					z->key[0] = x->key[i-1];
					x->key[i-1] = y->key[y->n - 1];
					z->cp[0] = y->cp[y->n];
					y->n -= 1;
					z->n += 1; 
					deleteNode(z, k, tree);
				}
			}
		}
	}
}
//출력 함수
void Traverse(btNode* x, int depth) {
	int i = x->n;
	if(x->isLeaf == false){
		for (int j = 0; j < i; j++) {
			for (int j = 0; j < depth; j++) {
				printf("         ");
			}
			printf("key : %d ", x->key[j]);
		}
		printf("\n");
		for (int j = 0; j <= i; j++) {
			Traverse(x->cp[j], depth + 1);
		}
	}
	else if(x->isLeaf == true) {
		for (int j = 0; j < i; j++) {
			for (int j = 0; j < depth; j++) {
				printf("             ");
			}
			printf("key: %d ", x->key[j]);
			printf("data : %d ", x->data[j]);
		}
		printf("\n");
	}
}


// 출력(DFS)
void print_for_exam(struct btNode* cur) {
	if (cur->isLeaf) {
		for (int i = 0; i < cur->n; i++) {
			printf("[%5d, %5d]\n", cur->key[i], cur->data[i]);
		}
	}
	else {
		for (int i = 0; i < cur->n; i++) {
			print_for_exam(cur->cp[i]);
			printf("[%5d]\n", cur->key[i]);
		}
		print_for_exam(cur->cp[cur->n]);
	}
}

// 테스트
int main()
{
	int arr[80] = TEST;
	struct bTree* BTree = createTree();
	
	
	
	// TEST 1 CASE
	insertNode(BTree, 4, 4 * 1000);
	insertNode(BTree, 1, 1 * 1000);
	insertNode(BTree, 3, 3 * 1000);
	insertNode(BTree, 2, 2 * 1000);
	deleteNode(BTree->root, 4, BTree);
	deleteNode(BTree->root, 2, BTree);
	deleteNode(BTree->root, 3, BTree);
	printf("\n---- TEST1 ----\n");
	print_for_exam(BTree->root, 0);
	
	// TEST 2 CASE
	for (int i = 2; i <= 100; i++) {
		insertNode(BTree, i, i * 1000);
	}
	
	for (int i = 50; i <= 70; i++) {
		//print_all(BTree->root, 0);
		deleteNode(BTree->root, i, BTree);
	}
	//print_all(BTree->root, 0);
	printf("\n\n\n\n\n\n");
	printf("---- TEST2 ----\n");
	print_for_exam(BTree->root);
	
	// TEST3 CASE
	for (int i = 50; i <= 70; i++) {
		insertNode(BTree, i, i * 1000);
	}
	
	for (int i = 0; i < 80; i++) {
		deleteNode(BTree->root, arr[i], BTree);
	}

	printf("\n\n\n\n\n\n");
	printf("---- TEST3 ----\n");
	print_for_exam(BTree->root);
	
	printf("프로그램이 정상적으로 종료 되었음.");
	return 0;
	
}
