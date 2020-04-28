#define _CRT_SECURE_NO_WARNINGS
#include <malloc.h>
#include <stdio.h>
#include <string.h>

struct MeteoAlerts {
	char alert[100];
	int priority = 0;
};

struct AlertsHeap {
	MeteoAlerts* alerts;
	int capacity;	//number of maximum elements
	int no;		//number of actual elements
};

MeteoAlerts initAlert(const char* msg, int priority) {
	MeteoAlerts alert;
	strcpy(alert.alert, msg);
	alert.priority = priority;
	return alert;
}

AlertsHeap initHeap(int initialSize) {
	AlertsHeap heap;
	heap.capacity = initialSize;
	heap.no = 0;
	heap.alerts = (MeteoAlerts*)malloc(initialSize * sizeof(MeteoAlerts));

	MeteoAlerts emptyAlert = initAlert("", 0);

	for (int i = 0; i < initialSize; i++)
		heap.alerts[i] = emptyAlert;	//= works because we don't have pointers in MeteoAlerts
	return heap;
}

int getRightChildIndex(int index) {
	return (index + 1) * 2;
}

int getLeftChildIndex(int index) {
	return index * 2 + 1;
}

int getParentIndex(int index) {
	return (int)(index - 1) / 2;
}

void resizeHeap(AlertsHeap* heap) {
	MeteoAlerts* newArray = 
		(MeteoAlerts*)malloc((heap->capacity + heap->capacity / 2) * sizeof(MeteoAlerts));
	for (int i = 0; i < heap->no; i++) {
		newArray[i] = heap->alerts[i];		// = works because we don't have ....
	}
	free(heap->alerts);
	heap->alerts = newArray;
	heap->capacity = (heap->capacity + heap->capacity / 2);
}

void insert(AlertsHeap* heap, MeteoAlerts alert) {
	//check for available space
	if (heap->no == heap->capacity) {
		resizeHeap(heap);
	}

	//add the new value in the heap
	heap->alerts[heap->no] = alert; // = works because we don't have ....
	heap->no += 1;

	int currentNodeIndex = heap->no - 1;

	while (true) {
		if (currentNodeIndex == 0)
			break;
		int parentNodeIndex = getParentIndex(currentNodeIndex);
		//compare the values
		if (heap->alerts[currentNodeIndex].priority > heap->alerts[parentNodeIndex].priority) {
			//interchange them
			MeteoAlerts temp = heap->alerts[currentNodeIndex];
			heap->alerts[currentNodeIndex] = heap->alerts[parentNodeIndex];
			heap->alerts[parentNodeIndex] = temp;
			currentNodeIndex = parentNodeIndex;
		}
		else
			break;
	}
}

void printMeteoAlert(MeteoAlerts alert) {
	printf("\n %d - %s", alert.priority, alert.alert);
}

void printHeap(AlertsHeap heap) {
	printf("\n Heap values: ");
	for (int i = 0; i < heap.no; i++) {
		printMeteoAlert(heap.alerts[i]);
	}
}

void changeValue(int* vb) {
	*vb = 90;
}

void changeValueCPP(int& vb) {
	vb = 90;
}

MeteoAlerts getAlert(AlertsHeap* heap) {
	MeteoAlerts result = heap->alerts[0];
	//replace the 1st value with the last one and do the comparisons downwards
	//...

	return result;
}

int main() {
	AlertsHeap heap = initHeap(100);
	MeteoAlerts alert1 = initAlert("Sunny weather", 5);
	insert(&heap, alert1);
	insert(&heap, initAlert("Rain",25));
	insert(&heap, initAlert("Havy Rain",50));
	insert(&heap, initAlert("Windy",25));
	insert(&heap, initAlert("Strong Wind",200));

	printHeap(heap);


}
