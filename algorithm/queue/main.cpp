#include "seqqueue.h"
#include "seqqueue.cpp"


int main()
{
	CSeqQueue<int, 100> queue;

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
