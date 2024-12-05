#ifndef QUEUE_H
#define QUEUE_H
//#define SIZE 5

constexpr int queue_size = 5; // #define SIZE 제거

class Queue {

private:
	int items[queue_size];
	int front;
	int rear;
public:
	Queue();

	bool isFull();

	bool isEmpty();

	void enQueue(int element);

	int deQueue();

	void display();
};

#endif // QUEUE_H
