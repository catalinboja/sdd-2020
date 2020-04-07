#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct ErrorMessages {
	char* text = NULL;
	int id = -1;
};

//alternative
//struct HashTableLocation {
//	ErrorMessages message;
//	bool isEmpty = false;
//};

struct HashTable {
	ErrorMessages *table = NULL;
	int noValues = 0;
};

ErrorMessages initMessage(const char* msg, int id) {
	ErrorMessages message;
	message.text = (char*)malloc(strlen(msg) + 1);
	strcpy(message.text, msg);
	message.id = id;
	return message;
}

void printMessage(ErrorMessages message) {
	if (message.text != NULL)
		printf("\n Error %s with id = %d", message.text, message.id);
	else
		printf("\n No error with id = %d", message.id);
}

void copyMessage(ErrorMessages* dest, ErrorMessages src) {
	dest->id = src.id;
	//avoid memory leaks
	if (dest->text != NULL)
		free(dest->text);
	dest->text = (char*)malloc(strlen(src.text) + 1);
	strcpy(dest->text, src.text);
}

void deleteMessage(ErrorMessages* msg) {
	if (msg->text != NULL)
		free(msg->text);
	msg->text = NULL;
}

HashTable createHashTable(int initialSize) {
	HashTable hTable;
	hTable.noValues = initialSize;
	hTable.table = (ErrorMessages *)malloc(
		initialSize * sizeof(ErrorMessages));

	for (int i = 0; i < initialSize; i++){
		hTable.table[i].id = -1;	//our rule, for -1 it means it's empty
		hTable.table[i].text = NULL;
	}

	return hTable;
}

int getHashValue(int key, int N) {
	return key % N;
}

//insert into a hashtable with probing
//@return 0 if the key is already present in the table
//@return -1 if the table is full
//@return 1 if the value has been inserted
int insertMessage(HashTable hashTable, ErrorMessages message) {
	int index = getHashValue(message.id, hashTable.noValues);
	for (int i = index; i < hashTable.noValues; i++) {
		if (hashTable.table[i].id == -1) {
			//we found an empty location
			copyMessage(&(hashTable.table[i]), message);
			return 1;
		}
		else
		{
			if (hashTable.table[i].id == message.id)
				return 0;
		}
	}

	//no available locations from index -> table end
	//we start probing from the beginning
	for (int i = 0; i < index; i++) {
		if (hashTable.table[i].id == -1) {
			//we found an empty location
			copyMessage(&(hashTable.table[i]), message);
			return 1;
		}
		else
		{
			if (hashTable.table[i].id == message.id)
				return 0;
		}
	}

	//the table is full
	return -1;
}

//@return a message with id = -1 if the given id 
//	is not present in the table
ErrorMessages getMessage(HashTable hashTable, int id) {
	ErrorMessages result = initMessage("", -1);
	int index = getHashValue(id, hashTable.noValues);
	int initialIndex = index;
	while (true) {

		if (index >= hashTable.noValues)
			index = 0;

		if (hashTable.table[index].id == id) {
			copyMessage(&result, hashTable.table[index]);
			break;
		}
		else {
			index += 1;
		}

		if (index == initialIndex)
			break;
	}

	return result;
}

//TODO
//resize the table

//TODO
//a function that will compute the percentage of free locations
//under 30% you need to resize

//TODO
//a function that will delete (logic delete) a message for a given id



int main() {

	ErrorMessages error1 = initMessage("No _CRT_SECURE", 20);
	ErrorMessages error2 = initMessage("NUll pointer", 15);
	//a colission for id = 20
	ErrorMessages error3 = initMessage("NUll pointer", 121);

	printMessage(error1);
	printMessage(error2);


	HashTable messages = createHashTable(101);
	insertMessage(messages, error1);
	insertMessage(messages, error2);
	insertMessage(messages, error3);

	int resCode = insertMessage(messages, error2);
	printf("\n Result code for inserting 15: %d", resCode);

	printf("\n --------------");

	printMessage(getMessage(messages, 15));
	printMessage(getMessage(messages, 20));
	printMessage(getMessage(messages, 53));
	printMessage(getMessage(messages, 121));

}