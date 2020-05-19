#include <stdio.h>
#include <malloc.h>

struct BSTNode {
	BSTNode* right;
	BSTNode* left;

	int key;
};

BSTNode* initNode(int key) {
	BSTNode* newNode = (BSTNode*)malloc(sizeof(BSTNode));
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->key = key;
	return newNode;
}

void printTreeInOrder(BSTNode* root) {
	if (root != NULL) {
		printTreeInOrder(root->left);
		printf(" %d ", root->key);
		printTreeInOrder(root->right);
	}
	return;
}

void printTreePreOrder(BSTNode* root) {
	if (root != NULL) {
		printf(" %d ", root->key);
		printTreeInOrder(root->left);
		printTreeInOrder(root->right);
	}
	return;
}

int getNodesCount(BSTNode* root) {
	if (root == NULL)
		return 0;
	return 1 + getNodesCount(root->right) + getNodesCount(root->left);
}

int max(int a, int b) {
	return a > b ? a : b;
}

int getTreeHeight(BSTNode* root) {
	if (root == NULL)
		return 0;
	else
		return 1 + max(getTreeHeight(root->left), getTreeHeight(root->right));
}

int getBalanceFactor(BSTNode* aNode) {
	return getTreeHeight(aNode->right) - getTreeHeight(aNode->left);
}


BSTNode* simpleRightRotation(BSTNode* pivot) {
	BSTNode* leftSibling = pivot->left;
	pivot->left = leftSibling->right;
	leftSibling->right = pivot;
	return leftSibling;
}

BSTNode* simpleLeftRotation(BSTNode* pivot) {
	BSTNode* rightSibling = pivot->right;
	pivot->right = rightSibling->left;
	rightSibling->left = pivot;
	return rightSibling;
}


BSTNode* insert(BSTNode * root, int key) {
	if (root == NULL)
		return initNode(key);

	if (root->key == key)
		return root;				//is a duplicate
	else
		if (root->key > key) {
			root->left = insert(root->left, key);
		}
		else {
			root->right = insert(root->right, key);
		}

	int balance = getBalanceFactor(root);
	printf("\n The balance factor for %d is %d", root->key, balance);

	//if the current node has a balance factor of -3 or 3
	if (balance == -3) {
		//check the unbalance of the left sibling
		if (getBalanceFactor(root->left) < 0) {
			//we do a simple rotation to the right on the current node
			return simpleRightRotation(root);
		}
		else
		{
			//we do a double rotation
		}
	}

	if (balance == 3) {
		//check the unbalance of the right sibling
		if (getBalanceFactor(root->right) > 0) {
			//we do a simple rotation to the left on the current node
			return simpleLeftRotation(root);
		}
		else
		{
			//we do a double rotation
		}
	}

	return root;
}

int main() {
	BSTNode* root = insert(NULL, 10);
	printf("\n --------------");
	root = insert(root, 9);
	printf("\n --------------");
	root = insert(root, 5);
	printf("\n --------------");
	root = insert(root, 3);
	printf("\n --------------");
	root = insert(root, 2);

	printf("\n The tree is");
	printTreeInOrder(root);

	printf("\n The tree height is %d", getTreeHeight(root));
	printf("\n Nodes count is %d", getNodesCount(root));
}