#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>


const float MAX_VALUE = 10000;
const int MAX_AGE = 65;
const int MIN_AGE = 20;
const int MAX_CATEGORY_CODE = 20;

const char* names[] = { "Alice", "John", "Bob", "Leo", "Anna", "Bianca", "Rose", "David" };
const int NAMES_NO = 8;

struct Customer {
	char* name;
	int age;
	int id;
	float cartValue;
	int categoryCode;
};

const char* getRandomNumber() {
	int index = rand() % NAMES_NO;
	return names[index];
}

//0.5p define a functions that will generate a random age (between MIN_AGE and MAX_AGE)

//0.5p define a function that will generate a random cart value (between 0 and MAX_VALUE)

//0.5p define a function that will generate a random category code value (between 0 and MAX_CATEGORY_CODE)

//0.5p define a function that will generate a dynamic array of Customers (the customers number is given; 
//		each customer has a unique ID and the rest of attributes are initialized with the previous functions)


//define a function that generates a Customer with random generated data



//implement the rest of the requested methods 





void main() {

	const int ARRAY_SIZE = 10000;
	//Customer testArray[ARRAY_SIZE];

	Customer * testArray = (Customer*)malloc(ARRAY_SIZE * sizeof(Customer));

	//leave this
	srand(1000);

	/*
		example of measuring performance
		you can remove it
	*/

	int* array = (int*)malloc(sizeof(int) * ARRAY_SIZE);

	clock_t start, end;
	double cpu_time_used;

	//record start time
	start = clock();

	for (int i = 0; i < ARRAY_SIZE; i++) {
		int r = rand() % ARRAY_SIZE;      // Returns a pseudo-random integer between 0 and ARRAY_SIZE.
		float s = r / 3;
		array[i] = s;
	}

	//record end time
	end = clock();
	//compute duration
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

	printf("Time is %f seconds", cpu_time_used);
}