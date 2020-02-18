#include <stdio.h>

struct valueTypeA {
	int value;
};

struct valueTypeB {
	int *pointerValue;
};

struct complexValue {
	int x;
	valueTypeA  a;
	valueTypeB* pointer2B;
};

struct impossibleStructure {
	complexValue* complexValues;
	complexValue** pointers2ComplexValues;
};

int main() {
	//a static array
	int values[10];

	//change the 5th value
	values[4] = 90;

	//pointer to a integer value
	int* pointer2Int = &values[4];

	printf("\n The value at that address is %d", *pointer2Int);

	pointer2Int += 3;
	*pointer2Int = 100;
	printf("\n The value at that address is %d", *pointer2Int);

	pointer2Int -= 6;
	*pointer2Int = 20;

	pointer2Int = &values[8];


	printf("\n");
	for (int i = 0; i < 10; i++)
		printf(" %d ", values[i]);

	//define a type A variable
	valueTypeA variableA;

	variableA.value = 100;
	valueTypeA* pointer2A;
	pointer2A = new valueTypeA();
	(*pointer2A).value = 100;
	pointer2A->value = 100;

	complexValue complex;
	complex.a.value = 60;
	complex.pointer2B = new valueTypeB();
	complex.pointer2B->pointerValue = new int();
	*(complex.pointer2B->pointerValue) = 99;

	complexValue* pComplex = &complex;
	*(pComplex->pointer2B->pointerValue) = 50;

	pComplex->a.value = 1000;

	//using an impossibleSstructure pointers init
	// the pointerValue from the 5th complexValue inside
	//		complexValues

	impossibleStructure* thePointer;
	thePointer = new impossibleStructure();

	thePointer->complexValues = new complexValue[10];
	thePointer->complexValues[4].pointer2B = new valueTypeB();
	thePointer->complexValues[4].pointer2B->pointerValue =
		new int();
	*(thePointer->complexValues[4].pointer2B->pointerValue) = 50;
}