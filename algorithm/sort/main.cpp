#include "sort.h"
#include "sort.cpp"


int main()
{
	CSort<int> sort;

	int arrays[] = { 9, 2, 1, 5, 3, 7, 8, 0, 4, 6 };
	//sort.bubble(arrays, 10);
	//sort.insert(arrays, 10);
	//sort.select(arrays, 10);
	sort.shell(arrays, 10);
	sort.show(arrays, 10);

	return 0;
}