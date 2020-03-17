#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct Message {
	int clientID;
	int priority;
	char text[100];
};

struct dnode {
	Message msg;
	dnode* next = NULL;
	dnode* prev = NULL;
};

struct MessageLoop {
	dnode* loop;
	char serverName[50];
};

dnode* createNode(Message message) {
	dnode* newNode = (dnode*)malloc(1 * sizeof(dnode));
	newNode->msg.priority = message.priority;
	newNode->msg.clientID = message.clientID;
	strcpy(newNode->msg.text, message.text);

	newNode->next = NULL;
	newNode->prev = NULL;

	return newNode;
}

MessageLoop createMessageLoop(const char* name) {
	MessageLoop newLoop;
	newLoop.loop = NULL;
	strcpy(newLoop.serverName, name);
	return newLoop;
}

void printMessage(Message msg) {
	printf("\n Client ID = %d, Priority = %d - %s",
		msg.clientID, msg.priority, msg.text);
}

void printMessageLoop(MessageLoop msgLoop) {
	printf("\n Messages for %s", msgLoop.serverName);
	if (msgLoop.loop == NULL) {
		printf("\n *** There are no messages ");
		return;
	}

	for (dnode* p = msgLoop.loop; p != NULL; p = p->next) {
		printMessage(p->msg);
	}

}

dnode* peek(dnode* node) {
	//if (node->next == NULL)
	//	return NULL;
	//else
	//	return node->next;

	return node->next;
}

bool hasNext(dnode* node) {
	return node->next == NULL ? false : true;
}

//inserting a new message
//insert based on priority
void insertMessage(MessageLoop* mLoop, Message msg) {
	// 1. create the new node
	dnode* newNode = createNode(msg);
	if (mLoop->loop == NULL) {
		mLoop->loop = newNode;
		return;
	}

	dnode* p = mLoop->loop;
	while (p->next != NULL &&
		p->next->msg.priority < msg.priority) {
		p = p->next;
	}

	//check if p is the last
	if (peek(p) == NULL) {
		//p is the last node
		//insert at the end
		p->next = newNode;
		newNode->prev = p;
	}
	else
	{
		//p is in the list
		newNode->next = p->next;
		newNode->prev = p;
		p->next->prev = newNode;
		p->next = newNode;
	}
}

int main() {
	MessageLoop alerts = 
		createMessageLoop("SmartCity Air Sensors");
	printMessageLoop(alerts);

	Message m1 = { 100, 2, "Calea Dorobanti 15C" };
	Message m2 = { 100, 3, "Piata Victoriei 15C" };
	Message m3 = { 100, 3, "Calea Victoriei 15C" };
	Message m4 = { 100, 7, "Stefan cel Mare 15C" };
	Message m5 = { 100, 5, "Piata Unirii" };

	insertMessage(&alerts, m1);
	insertMessage(&alerts, m2);
	insertMessage(&alerts, m3);
	insertMessage(&alerts, m4);

	printMessageLoop(alerts);

	insertMessage(&alerts, m5);

	printMessageLoop(alerts);

}