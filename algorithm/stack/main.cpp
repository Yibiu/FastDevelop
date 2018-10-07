#include "seqstack.h"
#include "seqstack.cpp"
#include "list_stack.h"
#include "list_stack.cpp"


int main()
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

 	return 0;
}