// Linear
#include "./linear/seqlinear.h"
#include "./linear/seqlinear.cpp"
#include "./linear/single_list.h"
#include "./linear/single_list.cpp"
#include "./linear/circle_list.h"
#include "./linear/circle_list.cpp"
#include "./linear/double_list.h"
#include "./linear/double_list.cpp"
// Queue
#include "./queue/seqqueue.h"
#include "./queue/seqqueue.cpp"
#include "./queue/circle_queue.h"
#include "./queue/circle_queue.cpp"
// Sort
#include "./sort/sort.h"
#include "./sort/sort.cpp"
// Stack
#include "./stack/seqstack.h"
#include "./stack/seqstack.cpp"
#include "./stack/list_stack.h"
#include "./stack/list_stack.cpp"


void test_linear()
{
	//CSeqLinear<int, 100> linear;
	//CSingleList<int> linear;
	CCircleList<int> linear;
	//CDoubleList<int> linear;

	linear.clear();
	for (int i = 0; i < 10; i++) {
		linear.insert(0, i);
	}
	linear.show();

	linear.remove(0);
	linear.remove(0);
	linear.show();
}

void test_queue()
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
}

void test_sort()
{
	CSort<int> sort;

	int arrays[] = { 9, 2, 1, 5, 3, 7, 8, 0, 4, 6 };
	//sort.bubble(arrays, 10);
	//sort.insert(arrays, 10);
	//sort.select(arrays, 10);
	sort.shell(arrays, 10);
	sort.show(arrays, 10);
}

void test_stack()
{
	//CSeqStack<int, 100> stack;
	CListStack<int> stack;

	for (int i = 0; i < 10; i++) {
		stack.push(i);
	}
	stack.show();

	int value = 0;
	stack.pop(value);
	stack.show();
	stack.get_top(value);
	stack.show();
}

void josephus()
{
	int n, m;
	cout << "People Count:" << endl;
	cin >> n;
	cout << "Number Count:" << endl;
	cin >> m;

	CCircleList<int> linear;
	for (int i = 1; i <= n; i++) {
		linear.insert(i - 1, i);
	}
	cout << "Before calculating..." << endl;
	linear.show();

	cout << "Calculating..." << endl;
	link_node_t<int> *cur = linear.get_head();
	link_node_t<int> *pre = NULL;
	while (linear.length() > 1)
	{
		for (int i = 0; i < m; i++) {
			pre = cur;
			cur = cur->next;
		}

		cout << "Queue Out is: " << cur->data << endl;
		pre->next = cur->next;
		delete cur;
		cur = pre->next;
	}

	cout << "After calculating..." << endl;
	linear.show();

	int last = 0;
	linear.get(0, last);
	cout << " The last on is " << last << "!" << endl;
}

int main()
{
	//test_linear();
	//test_queue();
	//test_sort();
	//test_stack();
	josephus();

	return 0;
}

