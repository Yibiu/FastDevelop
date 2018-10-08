#include "circle_queue.h"


template <typename T, int size>
CCircleQueue<T, size>::CCircleQueue()
{
	_max_size = size;
	_front = 0;
	_rear = 0;
}

template <typename T, int size>
CCircleQueue<T, size>::~CCircleQueue()
{
}

template <typename T, int size>
int CCircleQueue<T, size>::capacity() const
{
	return _max_size;
}

template <typename T, int size>
int CCircleQueue<T, size>::length() const
{
	return ((_rear - _front + _max_size) % _max_size);
}

template <typename T, int size>
bool CCircleQueue<T, size>::is_empty() const
{
	return (_rear == _front);
}

template <typename T, int size>
bool CCircleQueue<T, size>::is_full() const
{
	return ((_rear + 1) % _max_size == _front);
}

template <typename T, int size>
void CCircleQueue<T, size>::clear()
{
	_front = 0;
	_rear = 0;
}

template <typename T, int size>
bool CCircleQueue<T, size>::queue(const T &x)
{
	if (is_full())
		return false;

	_data[_rear] = x;
	_rear = (_rear + 1) % _max_size;
	return true;
}

template <typename T, int size>
bool CCircleQueue<T, size>::dequeue(T &x)
{
	if (is_empty())
		return false;

	x = _data[_front];
	_front = (_front + 1) % _max_size;
	return true;
}

template <typename T, int size>
bool CCircleQueue<T, size>::get_front(T &x) const
{
	if (is_empty())
		return false;

	x = _data[_front];
	return true;
}

template <typename T, int size>
bool CCircleQueue<T, size>::get_back(T &x) const
{
	if (is_empty())
		return false;

	x = _data[_rear - 1];
	return true;
}

template <typename T, int size>
void CCircleQueue<T, size>::show() const
{
	cout << "{ ";
	for (int i = _front; i < _rear; i++) {
		cout << _data[i] << " ";
	}
	cout << "}" << endl;
}
