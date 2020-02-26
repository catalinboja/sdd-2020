//Seminar 2
// Recap on dynamic arrays
// Load & Create a dynamic array using a 
//		input text file

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>

//1. define the structure
struct intArray {
	int * values = NULL;
	int n = 0;
};

struct intFile {
	FILE* pf = NULL;
	char fileName[50];
	bool isOpen = false;
};

//a method for creating an intArray 
intArray createArray(int size) {
	intArray newArray;
	newArray.values = (int*)malloc(size * sizeof(int));
	newArray.n = size;
	return newArray;
}

intArray createArray(int size, int* values) {
	intArray newArray = createArray(size);
	for (int i = 0; i < size; i++) {
		newArray.values[i] = values[i];
	}

	//redudant - does the same thing as the previous for
	memcpy(newArray.values, values, 
		size * sizeof(int));

	return newArray;
}

//a method to print the intArray
void printArray(intArray theArray) {
	if (theArray.n > 0) {
		printf("\n The values are: ");
		for (int i = 0; i < theArray.n; i++)
			printf(" %d", theArray.values[i]);
	}
	else
		printf("The array is empty");
}

//a method to set a value from the array
bool setValue(intArray theArray, int index, int value) {
	if (index >= 0 && index < theArray.n) {
		theArray.values[index] = value;
		return true;
	}
	else return false;
}

//a method to create an intFile element and to open it
intFile openFile(const char* filePath, 
	const char* openMode = "r") {
	intFile file;
	file.pf = fopen(filePath, openMode);
	strcpy(file.fileName, filePath);
	//memcpy(file.fileName, filePath, 
	//	strlen(filePath) + 1);
	file.isOpen = file.pf != NULL ? true : false;
	return file;
}

//the method read an array from a text file
intArray readArray(intFile file) {
	intArray result;
	if (file.isOpen) {
		//read the number of values
		fscanf(file.pf, "%d",&result.n);
		result = createArray(result.n);
		for (int i = 0; i < result.n; i++) {
			int value;
			fscanf(file.pf, "%d", &value);
			setValue(result, i, value);
		}
		fclose(file.pf);
		file.isOpen = false;
	}

	return result;
}


int main() {
	intArray a1 = createArray(5);
	printArray(a1);
	int values[] = { 10,20,30,40,50 };
	intArray a2 = createArray(5, values);
	printArray(a2);

	if(setValue(a2, 100, 100))
		printArray(a2);

	intFile file = openFile("values.txt");
	a2 = readArray(file);
	printArray(a2);
}
