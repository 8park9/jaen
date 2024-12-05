#include "Queue.h"
#include<iostream>

using namespace std;

Queue::Queue() : front(-1), rear(-1) {}

bool Queue::isFull() {
	if (front == 0 && rear == queue_size - 1) {
		return true;
	}
	if (front == rear + 1) {
		return true;
	}
	return false;
}

bool Queue::isEmpty() {
	if (front == -1) {
		return true;
	}
	else {
		return false;
	}

}

void Queue::enQueue(int element) {

	if (isFull()) {
		cout << "Queue is full";
	}
	else {
		if (front == -1) {
			front = 0;
		}
		rear = (rear + 1) % queue_size;
		items[rear] = element;
		cout << "inserted" << element << endl;
	}
}

int Queue::deQueue() {
	int element;
	if (isEmpty()) {
		cout << "Queue is empty" << endl;
		return(-1);
	}
	else {
		element = items[front];
		if (front == rear) {
			front = -1;
			rear = -1;
		}
		else {
			front = (front + 1) % queue_size;
		}
		return (element);
	}
}

void Queue::display() {
	int i;
	if (isEmpty()) {

		cout << endl
			<< "Empty Queue" << endl;
	}
	else {
		cout << "Front -> " << front;
		cout << endl
			<< "Items -> ";
		for (i = front; i != rear; i = (i + 1) % queue_size) {
			cout << items[i];
		}
		cout << items[i];
		cout << endl
			<< "Rear -> " << rear;
	}
}
