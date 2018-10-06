#include "seqlinear.h"
#include "seqlinear.cpp"
#include "single_list.h"
#include "single_list.cpp"
#include "circle_list.h"
#include "circle_list.cpp"
#include "double_list.h"
#include "double_list.cpp"


int main()
{
	//CSeqLinear<int, 100> linear;
	//CSingleList<int> linear;
	//CCircleList<int> linear;
	CDoubleList<int> linear;

	linear.clear();
	for (int i = 0; i < 10; i++) {
		linear.insert(0, i);
	}
	linear.show();

	linear.remove(0);
	linear.remove(0);
	linear.show();

	return 0;
}