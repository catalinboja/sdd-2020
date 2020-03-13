#include "Assignment1SDD.h"

/*
#1
implement a method that creates a new package with the given values
 
@return a package with the given values; by default the package is not delivered
@input data for package fields
*/
package createPackage(const char* street, int streetNo, float weight, const char* contact);


/*
#2
implement a method that prints a package at the console

@input the package to print
*/
void printPackage(package myPackage);

/*
#3
implement a method that convert the package structure to the serialized_version

@return returns a serialized_package article based on the received package
@input an existing package
*/
serialized_package serializePackage(package p);


/*
#4
implement a method that convert the serialized_package structure to a package one
@return returns a package article based on the received serialized_package
@input an existing serialized_package

Important ! watch out for the dynamic field in the package structure

*/
package deserializePackage(serialized_package s_p);

/*
#5
method for inserting a serialized_package element in the database

@return number of inserted packages
@input the database and the serialized package

*/
int insert(db myDb, serialized_package pack);

/*
#6
method for inserting an array of serialized_package elements in the database

@return number of inserted packages
@input the database, the array and its number of elements; the array can have fomr 1 to N packages

*/
int insert(db myDb, serialized_package* pack, int noPacks);

/*
#7
method for selecting the first package for a given contact name

@return the first package with the given contact; if there is no package for the contact then the functions
		return a package with a "non existing" contact
@input the database, the array and its number of elements
*/
package selectFirstOrDefault(db myDb, const char* contact);

/*
#8
method for selecting all

@input the database and the given weight
@return the packageList of the packages with a weight greater than the given one
*/
packageList selectHeavyPackages(db myDb, float weight);

/*
#9
method for printing to the console all packages from a package list

@input the given packageList
*/
void printAll(packageList list);


/*
#10
method for printing to the console all packages from the database

@input the database
*/
void printAll(db myDb);


int main() {

	/*

	*************************			Test Data

	*/
	//test data
	const char* streets[] = { "Calea Dorobanti", "Calea Victoriei", "Eminescu" };
	int numbers[] = { 10, 5, 87 };
	const char* contacts[] = { "John", "Anna", "Bob" };
	int weights[] = { 10.6, 3.5, 80 };


	//generate data
	package testData[3];
	for (int i = 0; i < 3; i++) {
		testData[i] = createPackage(streets[i], numbers[i], weights[i], contacts[i]);
	}

	//print test data
	printf("\n *Test Data*");
	for (int i = 0; i < 3; i++) {
		printPackage(testData[i]);
	}

	/*
	
	*************************			Part 1

	*/
	//generate the binary file
	db packageDB = createDB("admin", "1234", "TestPack");
	if (connect(&packageDB, "admin", "1234")) {
		printf("\n *Info* You are connected");

		for (int i = 0; i < 3; i++) {
			//insert the serialized version
			insert(packageDB, serializePackage(testData[i]));
		}

		//reset the cursor at the beginning
		reset(packageDB);

		printf("\n Size of the serialized article is %d bytes", sizeof(serialized_package));
		printf("\n File size is %d bytes", getFileSize(packageDB.pf));

		//reset the cursor at the beginning
		reset(packageDB);

		//print the file content
		printf("\n *File Data*");
		printAll(packageDB);

		close(&packageDB);
	}
	else
		printf("\n *Error* No connection to the DB");


	/*

	*************************			Part 2

	*/
	//load the given data
	db inputDb = createDB("admin", "1234", "InputPack");
	if (connect(&inputDb, "admin", "1234")) {
		printf("\n *Info* You are connected");

		//print the file content
		printf("\n *File Data*");
		printAll(inputDb);

		//reset the cursor at the beginning of the file
		reset(inputDb);

		package item = selectFirstOrDefault(inputDb, "John");

		printf("\n The searched package is for %s and its weight is %f", item.contact, item.weight);

		//reset the cursor at the beginning of the file
		reset(inputDb);

		packageList list = selectHeavyPackages(inputDb, 3.5);

		printAll(list);

		close(&packageDB);
	}
	else
		printf("\n *Error* No connection to the DB");

}