#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct Product {
	char barCode[500];
	char* description = NULL;
	int code = 0;
	float price = 0;
	int stock = 0;
};

//data storing & searching structure
struct BSNode {
	BSNode* right = NULL;
	BSNode* left = NULL;
	Product product;
};

//data index structure - only for searching (you don't store the data here, only the key)
struct BSIndexNode {
	BSNode* right = NULL;
	BSNode* left = NULL;
	int productCode;			//the key associated with the value
	long fileOffset;			//file location
};

Product initProduct(const char* barcode, const char* description,
	float price, float stock, int code) {
	Product product;
	product.code = code;
	product.price = price;
	product.stock = stock;
	strcpy(product.barCode, barcode);
	product.description = (char*)malloc(strlen(description) + 1);
	strcpy(product.description, description);
	return product;
}

void copyProduct(Product* destination, Product source) {
	destination->code = source.code;
	destination->price = source.price;
	destination->stock = source.stock;
	strcpy(destination->barCode, source.barCode);
	if (destination->description != NULL)
		free(destination->description);
	destination->description = (char*)malloc(strlen(source.description) + 1);
	strcpy(destination->description, source.description);
}

void printProduct(Product p) {
	printf("\n Product with id = %d and barcode = %s has a price of %f and the stock is %d",
		p.code, p.barCode, p.price, p.stock);
	if (p.description != NULL)
		printf("\n Description: %s", p.description);
}

BSNode* createNode(Product p) {
	BSNode* newNode = (BSNode*)malloc(sizeof(BSNode));
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->product.description = NULL;
	copyProduct(&(newNode->product), p);
	return newNode;
}


void printBST_Preorder(BSNode* root) {
	if (root == NULL)
		return;
	printBST_Preorder(root->left);
	printProduct(root->product);
	printBST_Preorder(root->right);
}

void printBST_Inorder(BSNode* root) {
	if (root == NULL)
		return;
	printProduct(root->product);
	printBST_Inorder(root->left);
	printBST_Inorder(root->right);
}

void insert(BSNode* * root, Product p) {

	if (*root == NULL) {
		*root = createNode(p);
		return;
	}

	//check for a duplicate key
	if ((*root)->product.code == p.code) {
		//copyProduct(&((*root)->product), p); // update
		return;
	}

	if ((*root)->product.code > p.code) {
		insert(&((*root)->left), p);
	}
	else
	{
		insert(&((*root)->right), p);
	}
}

//BSNode* insert(BSNode* root, Product p) {
//
//}

Product search(BSNode* node, int code) {
	Product result = initProduct("", "", 0, 0, -1);
	if (node == NULL)
		return result;
	if (code == node->product.code) {
		copyProduct(&result, node->product);
		return result;
	}
	else
		if (code > node->product.code) {
			return search(node->right, code);
		}
		else
			return search(node->left, code);
}


int main() {

	BSNode* products = NULL;
	Product p1 = initProduct("2AAB34", "", 2.3, 100, 10);
	Product p2 = initProduct("X20", "", 2.3, 50, 20);
	Product p3 = initProduct("X3", "", 2.3, 50, 3);
	Product p4 = initProduct("X1", "", 2.3, 50, 1);

	insert(&products, p1);
	insert(&products, p2);
	insert(&products, p3);
	insert(&products, p4);
	insert(&products, p1);

	printBST_Inorder(products);
	printf("\n ----------------- ");
	printBST_Preorder(products);
	printf("\n ----------------- ");

	Product result = search(products, 3);
	printProduct(result);
	result = search(products, 300);
	printProduct(result);
}
