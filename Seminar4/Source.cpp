#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Student {
	char name[50];
	int age;
};

struct StudentArray {
	Student * students;
	int noStudents;
};

struct node {
	Student student;
	node *next = NULL;
};


struct StudentsList {
	node* theFirst = NULL;
};

void printStudent(Student stud) {
	printf("\n %s is %d years old", stud.name, stud.age);
}

void printStudentList(StudentsList list) {
	node* iterator;
	iterator = list.theFirst;
	while (iterator != NULL) {
		printStudent(iterator->student);
		iterator = iterator->next;
	}

	//for (iterator = list.theFirst; iterator != NULL; iterator = iterator->next) {
	//	printStudent(iterator->student);
	//}
}

void insertAtBeginning(StudentsList* list, Student newStudent) {
	node* newNode = (node*)malloc(sizeof(node));
	newNode->next = NULL;
	newNode->student = newStudent;	//works because Student DOES NOT have dynamic fields

	newNode->next = list->theFirst;
	list->theFirst = newNode;
}


int main(int argc, char* argv[]) {

	printf("\n I am %s", argv[0]);

	//static array
	Student students[1000];

	//dynamic array
	int noStudents = 100;

	// check if we received the numbers students
	if (argc == 2) {
		noStudents = atoi(argv[1]);
	}

	printf("\n We have %d students", noStudents);

	Student *allStudents = (Student*)malloc(
		noStudents * sizeof(Student));

	//create a list of Students
	StudentsList list;
	printf("\n ***********************");
	printStudentList(list);

	Student s1 = { "John", 21 };
	Student s2 = { "Alice", 21 };

	insertAtBeginning(&list, s1);
	insertAtBeginning(&list, s2);

	printStudentList(list);

}