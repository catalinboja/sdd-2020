#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

const int NAME_SIZE = 1000;
const int FACULTY_SIZE = 500;
const int MAX_GRADES_NO = 50;

struct Student {
	int id;
	char* name;
	char* faculty;
	int* grades;
	int noGrades;
};



//needed for the direct access binary file
struct FixSizeStudent {
	int id;
	char name[NAME_SIZE];
	char faculty[FACULTY_SIZE];
	int grades[MAX_GRADES_NO];
	int noGrades;
};

Student initStudent(int id, const char* name, const char* faculty,
	int* grades, int noGrades) {
	Student student;
	student.id = id;
	student.name = (char*)malloc(strlen(name) + 1);
	strcpy(student.name, name);
	student.faculty = (char*)malloc(strlen(faculty) + 1);
	strcpy(student.faculty, faculty);

	if (grades == NULL || noGrades == 0) {
		student.grades = NULL;
		student.noGrades = 0;
	}
	else {
		student.grades = (int*)malloc(sizeof(int) * noGrades);
		memcpy(student.grades, grades, noGrades * sizeof(int));
		student.noGrades = noGrades;
	}
	return student;
}

void printStudent(Student student) {
	printf("\n %s - %s", student.name, student.faculty);
	printf("\n Grades:");
	for (int i = 0; i < student.noGrades; i++)
		printf(" %d", student.grades[i]);
}

FixSizeStudent convert2FixSize(Student student) {
	FixSizeStudent fixSizeStudent;

	strcpy(fixSizeStudent.name, student.name);
	strcpy(fixSizeStudent.faculty, student.faculty);
	fixSizeStudent.id = student.id;
	fixSizeStudent.noGrades = student.noGrades;
	for (int i = 0; i < student.noGrades; i++) {
		fixSizeStudent.grades[i] = student.grades[i];
	}
	return fixSizeStudent;
}

Student convert2Student(FixSizeStudent fixStudent) {
	Student student = initStudent(fixStudent.id,
		fixStudent.name,
		fixStudent.faculty,
		fixStudent.grades,
		fixStudent.noGrades);
	return student;
}

struct StudentIndexNode {
	int id;
	long long offset = 0;
	StudentIndexNode * right = NULL;
	StudentIndexNode * left = NULL;
};

StudentIndexNode * initNode(Student student) {
	StudentIndexNode * newNode = (StudentIndexNode*)malloc(sizeof(StudentIndexNode));
	newNode->id = student.id;
	newNode->offset = -1;		//it means the student is not stored in the file
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}

void setNodeOffset(StudentIndexNode* node, long long offset) {
	node->offset = offset;
}

struct BinaryFile {
	char name[50];
	FILE* pf;
};

BinaryFile openFile(const char* name) {
	BinaryFile file;
	strcpy(file.name, name);
	file.pf = fopen(file.name, "a+b");			//append mode and binary
	return file;
}

void closeFile(BinaryFile* file) {
	fclose(file->pf); 
	file->pf = NULL;
}

bool isOpen(BinaryFile file) {
	return file.pf == NULL ? false : true;
}

long long writeStudentIntoFile(const char* fileName, Student student) {
	FixSizeStudent fixStudent = convert2FixSize(student);

	BinaryFile file = openFile(fileName);

	if (file.pf == NULL)
		return -1;	//we don't have access to that file

	//get the offset at the end of the file
	fseek(file.pf, 0, SEEK_END);
	long long offset = ftell(file.pf);
	fwrite(&fixStudent, sizeof(FixSizeStudent), 1, file.pf);

	closeFile(&file);

	return offset;
}

int updateStudentFromFile(const char* fileName, Student newStudenValue, long long offset) {
	FixSizeStudent fixStudent = convert2FixSize(newStudenValue);
	BinaryFile file = openFile(fileName);

	if (file.pf == NULL)
		return -1;	//we don't have access to that file

	//get the current offset
	//we position the cursor at the end of the file
	fseek(file.pf, 0, SEEK_END);
	long long currentOffset = ftell(file.pf);
	if (offset >= currentOffset)
		return -2;	//the received offset is not valid  - is outside of the file

	//change the file location to the given offset
	fseek(file.pf, offset, SEEK_SET);

	//update the existing student
	fwrite(&fixStudent, sizeof(FixSizeStudent), 1, file.pf);

	closeFile(&file);

	return 1;
}

Student readStudentFromFile(const char* fileName, long long offset) {

	Student result = initStudent(-1, "", "", NULL, 0);

	BinaryFile file = openFile(fileName);
	if (!isOpen(file))
		return result;

	fseek(file.pf, 0, SEEK_END);
	long long currentOffset = ftell(file.pf);
	if (offset >= currentOffset)
		return result;

	//locate the student
	fseek(file.pf, offset, SEEK_SET);

	//read the student
	FixSizeStudent fixStudent;
	fread(&fixStudent, sizeof(FixSizeStudent), 1, file.pf);

	result = convert2Student(fixStudent);

	return result;
}


struct PrimaryKey {
	StudentIndexNode* root;
	char keyName[50];
	char tableName[50];
};

PrimaryKey createKey(const char* column, const char* table) {
	PrimaryKey key;
	strcpy(key.keyName, column);
	strcpy(key.tableName, table);
	key.root = NULL;
	return key;
}

StudentIndexNode* insertStudent(StudentIndexNode* root, Student student, const char* fileName) {
	if (root == NULL) {
		StudentIndexNode *newNode = initNode(student);
		long long offset = writeStudentIntoFile(fileName, student);
		newNode->offset = offset;
		return newNode;
	}

	//check if the student is already here
	if (root->id == student.id) {
		//update the student values into the file
		updateStudentFromFile(fileName, student, root->offset);
		return root;
	}

	//searching for a possible location
	if (root->id > student.id) {
		root->left = insertStudent(root->left, student, fileName);
	}
	else {
		root->right = insertStudent(root->right, student, fileName);
	}

	return root;
}

Student search(StudentIndexNode* root, int keyValue, const char* fileName) {
	Student result = initStudent(-1, "", "", NULL, 0);	//TO DO update init for NULL
	
	if (root == NULL) {
		return result;
	}
	
	if (root->id == keyValue) {
		//read the student data from the file
		result = readStudentFromFile(fileName, root->offset);
		return result;
	}

	if (root->id > keyValue) {
		return search(root->left, keyValue, fileName);
	}
	else
		return search(root->right, keyValue, fileName);

}

int main() {

	int grades[] = { 9,9,9 };
	int noGrades = 3;

	PrimaryKey idKey = createKey("id", "students");
	idKey.root = insertStudent(
		idKey.root, initStudent(100, "John", "CSIE", NULL, 0), "students.db");
	idKey.root = insertStudent(
		idKey.root, initStudent(50, "Alice", "CSIE", grades, noGrades), "students.db");
	idKey.root = insertStudent(
		idKey.root, initStudent(150, "Bob", "MK", grades, noGrades), "students.db");


	Student student = search(idKey.root, 34, "students.db");
	if (student.id != -1)
		printStudent(student);
	else
		printf("No value for key = 34");


	//we loose the index
	//the index should be written in an another file
	printf("\n The end");
}