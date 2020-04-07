#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <string.h>
#include <stdio.h>

struct Product {
	char description[50];
	float price;
	int stock;
	char barCode[50];
};


struct Node {
	Product product;
	Node* next = NULL;
};

struct HashTable {
	Node* * arrayOfLists = NULL;
	int noElements;
};

struct ProductsCatalog {
	char storeName[50];
	HashTable catalog;
};

Product createProduct(const char* description,
	float price, int stock, const char* barcode) {

	Product product;
	strcpy(product.description, description);
	product.price = price;
	product.stock = stock;
	strcpy(product.barCode, barcode);
	return product;
}

//if you have pointers in your structure
//	use this function to copy them instead of = 
Product copyProduct(Product p);

Node* createNode(Product product) {
	Node* newNode = (Node*)malloc(sizeof(Node));
//because we don't have pointers in Product
newNode->product = product;
newNode->next = NULL;
return newNode;
}

HashTable initHashTable(int initialSize) {
	HashTable hashTable;
	hashTable.noElements = initialSize;
	hashTable.arrayOfLists =
		(Node* *)malloc(initialSize * sizeof(Node*));
	for (int i = 0; i < hashTable.noElements; i++)
		hashTable.arrayOfLists[i] = NULL;
	return hashTable;
}

ProductsCatalog initCatalog(const char* storeName) {
	ProductsCatalog dictionary;
	strcpy(dictionary.storeName, storeName);
	dictionary.catalog = initHashTable(101);
	return dictionary;
}

void printProduct(Product product) {
	printf("\n Product: %s | price %f | stock %d | barcode %s",
		product.description, product.price, product.stock,
		product.barCode);
}

//THE hash function
//@input it will take the product barcode 
//@output the index of the list that may have or not the searched product
//		or the product that we need to insert
//the returned index is between [0, arrayLength-1]
int hashFunction(const char* barcode, int tableSize) {
	int barcodeAsciiSum = 0;

	for (int i = 0; i < strlen(barcode); i++) {
		barcodeAsciiSum += barcode[i];
	}

	int index = barcodeAsciiSum % tableSize;

	return index;
}


Node* insertAtHead(Node* head, Product product) {
	Node* newNode = createNode(product);
	newNode->next = head;
	return newNode;
}

//function that deletes a simple linked list
void deleteList(Node* head) {
	Node* temp = head;
	while (temp != NULL) {
		Node* p = temp;
		temp = temp->next;
		free(p);
	}
}

void resizeHashTable(HashTable* hashTable) {
	int newSize = hashTable->noElements * 2 + 1;
	Node* *newArray = (Node* *)malloc(newSize * sizeof(Node*));
	for (int i = 0; i < newSize; i++) {
		newArray[i] = NULL;
	}

	//we go through the previous hashTable and reinsert all existing products
	//go list by list
	for (int i = 0; i < hashTable->noElements; i++) {
		//go Product by Product in the current list
		for (Node* p = hashTable->arrayOfLists[i];
			p != NULL; p = p->next) {
			int newIndex = hashFunction(p->product.barCode, newSize);
			newArray[newIndex] =
				insertAtHead(newArray[newIndex], p->product);
		}
	}

	//delete the lists
	for (int i = 0; i < hashTable->noElements; i++)
		deleteList(hashTable->arrayOfLists[i]);
	//delete the array
	free(hashTable->arrayOfLists);

	hashTable->noElements = newSize;
	hashTable->arrayOfLists = newArray;
}


Product* searchForProduct(Node* head, const char* barcode) {
	if (head == NULL)
		return NULL;
	for (Node* p = head; p != NULL; p = p->next) {
		if (strcmp(p->product.barCode, barcode) == 0)
			return &p->product;
	}
	return NULL;
}

//@return 0 for empty list
//@return 1 for new inserted product
//@return 2 for a product stock update
int insertProductIntoCatalog(ProductsCatalog dictionary, Product product) {
	if (dictionary.catalog.arrayOfLists == NULL)
		return 0;
	int listIndex = hashFunction(product.barCode,
		dictionary.catalog.noElements);
	//check for duplicates
	Product *existing =
		searchForProduct(dictionary.catalog.arrayOfLists[listIndex],
			product.barCode);
	if (existing!=NULL) {
		//the product is already in the table
		//we update the stock
		existing->stock += product.stock;
		return 2;
	}
	else
	{
		//it's a new product
		//let's insert it
		dictionary.catalog.arrayOfLists[listIndex] =
			insertAtHead(dictionary.catalog.arrayOfLists[listIndex],
				product);
		return 1;
	}
}

Product searchCatalog(ProductsCatalog pd, const char* barcode) {
	int listIndex = hashFunction(barcode, pd.catalog.noElements);
	Product* existing =
		searchForProduct(pd.catalog.arrayOfLists[listIndex], barcode);
	if (existing == NULL) {
		return createProduct("None", -1, -1, "");
	}
	else
		return *existing;
}




int main() {

	ProductsCatalog storeCatalog = initCatalog("My store");
	Product p1 = { "Soda",3.3, 100, "2A2345" };
	Product p2 = { "Bread",2, 1000, "456AB" };
	Product p3 = { "Salad",9.6, 50, "1234ZX" };

	insertProductIntoCatalog(storeCatalog, p1);
	insertProductIntoCatalog(storeCatalog, p2);
	insertProductIntoCatalog(storeCatalog, p3);

	printProduct(searchCatalog(storeCatalog, "456AB"));
	printProduct(searchCatalog(storeCatalog, "1234ZX"));
	printProduct(searchCatalog(storeCatalog, "AAAAAA23"));

	resizeHashTable(&storeCatalog.catalog);

	printf("New size is %d", storeCatalog.catalog.noElements);
	printProduct(searchCatalog(storeCatalog, "456AB"));
	printProduct(searchCatalog(storeCatalog, "1234ZX"));
	printProduct(searchCatalog(storeCatalog, "AAAAAA23"));

}

