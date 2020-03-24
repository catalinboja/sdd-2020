#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct Message {
	char text[100];
	int clientId;
};

struct node {
	Message msg;
	node* next;
};

struct MessagesRingBuffer {
	char* bufferName = NULL;
	node* head = NULL;
	node* tail = NULL;
};

Message initMessage(const char* txt, int clientId) {
	Message msg;
	strcpy(msg.text, txt);
	msg.clientId = clientId;
	return msg;
}

node* initNode(Message msg) {
	node* newNode = (node*)malloc(sizeof(node));
	newNode->next = NULL;
	newNode->msg = msg;	//works because does not contain pointers
	return newNode;
}

MessagesRingBuffer initRingBuffer(const char* name) {
	MessagesRingBuffer buffer;
	buffer.head = NULL;
	buffer.tail = NULL;
	buffer.bufferName = (char*)malloc(strlen(name) + 1);
	strcpy(buffer.bufferName, name);
	return buffer;
}

void printMsg(Message msg) {
	printf("\n From client %d - %s", msg.clientId, msg.text);
}

void printRingBuffer(MessagesRingBuffer buffer) {
	if (buffer.bufferName != NULL) {
		printf("\n ******** %s content ********",
			buffer.bufferName);
	}
	else
		printf("\n Just a ring buffer ");

	if (buffer.head == NULL) {
		printf("\n It's empty");
		return;
	}

	node* it = buffer.head;
	while (it != NULL) {
		printMsg(it->msg);
		it = it->next;
	}

	//alternative
	//do {
	//	//will do this and after will check 
	//	printMsg(it->msg);
	//	it = it->next;
	//} while (it->next != NULL);
}

void addMsg(MessagesRingBuffer* buffer, Message msg) {
	
	node* newNode = initNode(msg);

	if (buffer->head == NULL) {
		//the list is empty case
		buffer->head = newNode;
		buffer->tail = newNode;
		return;
	}

	//insert at the end
	buffer->tail->next = newNode;
	buffer->tail = newNode;
}

Message getMsg(MessagesRingBuffer* buffer) {
	if (buffer->head == NULL) {
		//no messages
		Message msg = initMessage("", -1);
		return msg;
	}

	if (buffer->head == buffer->tail) {
		//only 1 message left
		Message msg = buffer->head->msg;
		free(buffer->head);
		buffer->head = buffer->tail = NULL;
		return msg;
	}

	//at least 2 messages left
	node* currentHead = buffer->head;
	buffer->head = buffer->head->next;
	Message msg = currentHead->msg;
	free(currentHead);
	return msg;
}

int main() {
	MessagesRingBuffer buffer = 
		initRingBuffer("Apple stock exchange");
	printRingBuffer(buffer);

	Message buy1 = { "Buy 1000", 1001 };
	Message buy2 = { "Buy 1000", 23 };
	Message sell1 = { "Sell 500", 50 };

	addMsg(&buffer, buy1);
	addMsg(&buffer, buy2);
	addMsg(&buffer, sell1);

	printRingBuffer(buffer);

	printf("\n Processing messages: ");
	Message m = getMsg(&buffer);
	printMsg(m);
	m = getMsg(&buffer);
	printMsg(m);

	printRingBuffer(buffer);


}
