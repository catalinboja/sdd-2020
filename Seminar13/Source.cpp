#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct MainListNode;

//secondary list that stores the destinations from a main list node
struct DestinationNode {
	DestinationNode* next;
	MainListNode *destination;
	float time;
};

//primary list that stores the distinct nodes from the graph
struct MainListNode {
	char destination[50];
	MainListNode* next;
	DestinationNode* destinations;
};

DestinationNode* initDestination(MainListNode* nodeAddress, float time) {
	DestinationNode* newNode = (DestinationNode*)malloc(sizeof(DestinationNode));
	newNode->next = NULL;
	newNode->time = time;
	newNode->destination = nodeAddress;
	return newNode;
}

MainListNode* initMainNode(const char* destination) {
	MainListNode* newNode = (MainListNode*)malloc(sizeof(MainListNode));
	newNode->next = NULL;
	newNode->destinations = NULL;
	strcpy(newNode->destination, destination);
	return newNode;
}

//insert a node in the main list
MainListNode* insertMainNode(MainListNode* listHead, const char* destination) {
	MainListNode* newNode = initMainNode(destination);
	if (listHead == NULL)
		return newNode;
	MainListNode* p = listHead;
	while (p->next != NULL)
		p = p->next;
	p->next = newNode;
	return listHead;
}

void printMainList(MainListNode* listHead) {
	printf("\n The main nodes from the graph are:");
	MainListNode* p = listHead;
	while (p != NULL) {
		printf("%s,", p->destination);
		p = p->next;
	}
}

//returns the main list node for a given destination
MainListNode* searchDestination(MainListNode* listHead, const char* destination) {
	if (listHead == NULL)
		return NULL;
	MainListNode* p = listHead;
	while (p != NULL) {
		if (strcmp(p->destination, destination) == 0)
			return p;
		p = p->next;
	}

	return NULL;
}


//insert in the secondary destinations list
void insertDestination(MainListNode** sourcePointer, MainListNode* destinationPointer, float time) {
	DestinationNode* destinationsList = (*sourcePointer)->destinations;
	DestinationNode* newDestination = initDestination(destinationPointer, time);

	newDestination->next = destinationsList;
	(*sourcePointer)->destinations = newDestination;
}

//2nd version
DestinationNode* insertDestination(DestinationNode* destinationsHead, MainListNode* destinationPointer, float time) {
	DestinationNode* newDestination = initDestination(destinationPointer, time);
	newDestination->next = destinationsHead;
	return newDestination;
}

void addRoute(const char* source, const char* destination, float time, MainListNode* graph) {
	MainListNode* sourceNode = searchDestination(graph, source);
	MainListNode* destinationNode = searchDestination(graph, destination);

	if (sourceNode == NULL || destinationNode == NULL)
		return;

	//version 1
	//insertDestination(&sourceNode, destinationNode, time);
	//version 2
	sourceNode->destinations = insertDestination(sourceNode->destinations, destinationNode, time);
}

void printGraph(MainListNode* graph) {
	printf("\n ---------------------------");
	printMainList(graph);
	MainListNode* p = graph;
	while (p != NULL) {
		printf("\n Destinations from %s ", p->destination);
		DestinationNode* q = p->destinations;
		while (q != NULL) {
			printf(" %s (%f), ", q->destination->destination, q->time);
			q = q->next;
		}

		p = p->next;
	}
}

//we will follow the first destination available from the current node until we reach a deadend
void justGo(const char* source, MainListNode* graph) {
	MainListNode* sourceNode = searchDestination(graph, source);
	if (sourceNode != NULL)
	{
		printf("We are now in %s ", sourceNode->destination);
		if(sourceNode->destinations != NULL)
			justGo(sourceNode->destinations->destination->destination, graph);
	}
}


int main() {
	const char* cities[] = { "Bucharest", "Ploiesti", "Comarnic", 
		"Brasov", "Valenii de Munte", "Buzau" };

	MainListNode* graph = NULL;

	for (int i = 0; i < 6; i++)
		graph = insertMainNode(graph, cities[i]);

	printMainList(graph);

	//add links between cities
	addRoute("Bucharest", "Buzau", 90, graph);
	addRoute("Bucharest", "Ploiesti", 40, graph);
	addRoute("Ploiesti", "Comarnic", 30, graph);
	addRoute("Ploiesti", "Valenii de Munte", 80, graph);
	addRoute("Comarnic", "Brasov", 80, graph);


	printGraph(graph);

	justGo(cities[0], graph);
}