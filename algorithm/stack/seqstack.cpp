#include "seqstack.h"


template <typename T, int size>
CSeqStack<T, size>::CSeqStack()
{
	_max_size = size;
	_top = -1;
}

template <typename T, int size>
CSeqStack<T, size>::~CSeqStack()
{
}

template <typename T, int size>
int CSeqStack<T, size>::capacity() const
{
	return _max_size;
}

template <typename T, int size>
int CSeqStack<T, size>::length() const
{
	return (_top + 1);
}

template <typename T, int size>
bool CSeqStack<T, size>::is_empty() const
{
	return (-1 == _top);
}

template <typename T, int size>
bool CSeqStack<T, size>::is_full() const
{
	return (_top == _max_size - 1);
}

template <typename T, int size>
void CSeqStack<T, size>::clear()
{
	_top = -1;
}

template <typename T, int size>
bool CSeqStack<T, size>::push(const T &x)
{
	if (is_full())
		return false;

	_top++;
	_data[_top] = x;
	return true;
}

template <typename T, int size>
bool CSeqStack<T, size>::pop(T &x)
{
	if (is_empty())
		return false;

	x = _data[_top];
	_top--;
	return true;
}

template <typename T, int size>
bool CSeqStack<T, size>::get_top(T &x) const
{
	if (is_empty())
		return false;

	x = _data[_top];
	return true;
}

template <typename T, int size>
void CSeqStack<T, size>::show() const
{
	cout << "{ ";
	for (int i = 0; i <= _top; i++) {
		cout << _data[i] << " ";
	}
	cout << "}" << endl;
}
