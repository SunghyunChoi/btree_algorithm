#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int order, t;

struct btNode {

	int key_num;
	int* keys;
	struct btNode** child_pointer;
	bool isLeaf;

};

struct bTree {

	//int order;
	struct btNode* root;
	int node_count;

};

typedef struct btNode btNode;
typedef struct bTree bTree;

btNode* create_node(int order, bool isLeaf) {
	btNode node;
	node.isLeaf = isLeaf;
	node.key_num = 0;
	node.keys = (int*)malloc((order - 1) * sizeof(int));
	node.child_pointer = (btNode*)malloc((order) * sizeof(struct btNode*));
	for (int i = 0; i < order - 1; i++) {
		(node.keys)[i] = -1;
	}
	return &node;
}
bTree* create_tree(int order) {

	bTree tree;
	//tree.order = order;
	tree.root = create_node(order, true);
	tree.node_count = 1;
	return &tree;

}

void insertNonFull(btNode* x, int k) {



}
void split(btNode* x, int i) {

	btNode* y;// 분할해줄 x의 자식 노드
	btNode* z;// 새로 생성될 x의 자식 노드
	y = x->child_pointer[i];
	z = create_node(order, y->isLeaf);
	z->key_num = t-1; 
	
	// y의 키들 중 t-1개를 z로 옮긴다.
	for (int j = 0; j < t - 1; j++) {
		z->keys[j] = y->keys[j + t];
	}

	//y가 leaf가 아니라면 자식노드가 존재하므로 자식 노드들도 t-1개 옮긴다.
	if (! y->isLeaf) {
		for (int j = 0; j < t; j++) {
			z->child_pointer[j] = y->child_pointer[j + t];
		}
	}
	y->key_num = t - 1;

	//자식 한개가 x로 올라가야 하므로, x의 기존 자식들을 다 한칸씩 오른쪽으로 밀어준다.
	for (int j = x->key_num - 1; j >= i; j--) {
		x->child_pointer[j + 1] = x->child_pointer[j];
	}
	x->child_pointer[i] = z;
	
	//키 한개가 x로 올라가야 하므로, x의 기존 자식들을 다 한칸씩 오른쪽으로 밀어준다.
	for (int j = x->key_num - 1; j >= i; j--) {
		x->keys[j + 1] = x->keys[j];
	}
	x->keys[i - 1] = y->keys[t - 1];
	x->key_num = x->keys + 1;
}
void tree_insert(bTree* tree, int k) { //tree에 k라는 key를 넣고 싶다.
	
	btNode* r = tree->root;
	
	//root부터 재귀적으로 들어가며 가득차있는지 확인한다.
	if (r->key_num == order - 1) {
		//root가 가득 차있다면, 새로운 노드를 생성하여 루트로 만든 후, 키값을 올린다.
		btNode* s;
		s = create_node(order, false);
		tree->root = s;
		s->key_num = 0;
		s->isLeaf = 0;
		(s->child_pointer)[0] = r;
		split(s, 0);
		insertNonFull(s, k);
	}
	else {
		insertNonFull(r, k);
	}

}

main() {

	printf("최대 차수를 입력하세요 : ");
	scanf_s("%d", &order);
	t = order / 2;
	btNode* n;
	n = create_node(5, false);
	printf("%d\n", (n->keys)[0]);
	
}