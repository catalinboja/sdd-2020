#include <vector>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;

string faculty[] = { "CSIE", "MAN", "FABBV", "MK", "COM", "CIG", "EAM", "FABIZ" };

string getRandomFaculty() {
	int index = rand() % 8;
	return faculty[index];
}

string getRandomName(vector<string> names) {
	int index = rand() % names.size();
	return names[index];
}

int getRandomNoGrades() {
	return rand() % 7;
}

void generareDate() {
	ifstream names("names.txt");
	vector<string> vNames;
	if (names.is_open()) {
		while (!names.eof()) {
			string name;
			names >> name;
			vNames.push_back(name);
		}
		names.close();
	}

	printf("\n Numar nume = %d", vNames.size());

	ofstream students("students.txt");

	srand(1000);

	for (int i = 0; i < 10000; i++) {
		students << i + 1 << endl;
		students << getRandomName(vNames) << endl;
		students << getRandomFaculty() << endl;
		int noGrades = getRandomNoGrades();
		students << noGrades << endl;
		for (int i = 0; i < noGrades; i++) {
			students << (rand() % 10 + 1) << endl;
		}
	}

	students.close();
}


struct Student {
	int studentId; 	// unique value assigned to each student
	char * name;
	int nrGrades;
	int * grades;
	char faculty[100];
};


/*

The application loads the data from the given text file.

5p out of 10

(1p) 1. Load students into a hash table implemented by probing. The function used to calculate the hash value is chosen freely.
(1p) 2. Implement a function that calculates the loading degree of the hash table (number of locations occupied per total).
(1p) 3. Implement / modify the function for  inserting a new element in the hash table so that if the loading degree exceeds 70% then the table will be resized. Note that the insertion function must not allow duplicate keys. If it is desired to insert an existing key then the data of the respective student will be updated.
(1p) 4. Implement a function that allows the deletion of a record from the dispersion table.
(0.5p) 5. Implement the method that allows university to get a student tby its id.
(0.5p) 6. Test the methods in main().


5p out of 10

To allow quick searches by faculty and student id  number (composite index) implement another hash table that uses chaining to store data by id number and faculty name. The composition of the two values ​​and the implementation of the hash function are left to the author of the solution.

Implement and test the requirements 1-6 also for this variant of implementation.


*/


int main() {
	//generareDate();

}