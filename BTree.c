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

	btNode* y;// �������� x�� �ڽ� ���
	btNode* z;// ���� ������ x�� �ڽ� ���
	y = x->child_pointer[i];
	z = create_node(order, y->isLeaf);
	z->key_num = t-1; 
	
	// y�� Ű�� �� t-1���� z�� �ű��.
	for (int j = 0; j < t - 1; j++) {
		z->keys[j] = y->keys[j + t];
	}

	//y�� leaf�� �ƴ϶�� �ڽĳ�尡 �����ϹǷ� �ڽ� ���鵵 t-1�� �ű��.
	if (! y->isLeaf) {
		for (int j = 0; j < t; j++) {
			z->child_pointer[j] = y->child_pointer[j + t];
		}
	}
	y->key_num = t - 1;

	//�ڽ� �Ѱ��� x�� �ö󰡾� �ϹǷ�, x�� ���� �ڽĵ��� �� ��ĭ�� ���������� �о��ش�.
	for (int j = x->key_num - 1; j >= i; j--) {
		x->child_pointer[j + 1] = x->child_pointer[j];
	}
	x->child_pointer[i] = z;
	
	//Ű �Ѱ��� x�� �ö󰡾� �ϹǷ�, x�� ���� �ڽĵ��� �� ��ĭ�� ���������� �о��ش�.
	for (int j = x->key_num - 1; j >= i; j--) {
		x->keys[j + 1] = x->keys[j];
	}
	x->keys[i - 1] = y->keys[t - 1];
	x->key_num = x->keys + 1;
}
void tree_insert(bTree* tree, int k) { //tree�� k��� key�� �ְ� �ʹ�.
	
	btNode* r = tree->root;
	
	//root���� ��������� ���� �������ִ��� Ȯ���Ѵ�.
	if (r->key_num == order - 1) {
		//root�� ���� ���ִٸ�, ���ο� ��带 �����Ͽ� ��Ʈ�� ���� ��, Ű���� �ø���.
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

	printf("�ִ� ������ �Է��ϼ��� : ");
	scanf_s("%d", &order);
	t = order / 2;
	btNode* n;
	n = create_node(5, false);
	printf("%d\n", (n->keys)[0]);
	
}