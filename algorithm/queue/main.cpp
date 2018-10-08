#include "seqqueue.h"
#include "seqqueue.cpp"
#include "circle_queue.h"
#include "circle_queue.cpp"


int main()
{
	//CSeqQueue<int, 100> queue;
	CCircleQueue<int, 100> queue;

	for (int i = 0; i < 10; i++) {
		queue.queue(i);
	}
	queue.show();

	int value = 0;
	queue.dequeue(value);
	queue.dequeue(value);
	queue.show();

	return 0;
}
