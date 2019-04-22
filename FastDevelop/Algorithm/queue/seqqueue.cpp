#include "seqqueue.h"


template <typename T, int size>
CSeqQueue<T, size>::CSeqQueue()
{
	_max_size = size;
	_front = 0;
	_rear = 0;
}

template <typename T, int size>
CSeqQueue<T, size>::~CSeqQueue()
{
}

template <typename T, int size>
int CSeqQueue<T, size>::capacity() const
{
	return _max_size;
}

template <typename T, int size>
int CSeqQueue<T, size>::length() const
{
	return (_rear - _front);
}

template <typename T, int size>
bool CSeqQueue<T, size>::is_empty() const
{
	return (_rear == _front);
}

template <typename T, int size>
bool CSeqQueue<T, size>::is_full() const
{
	return (0 == _front && _rear == _max_size);
}

template <typename T, int size>
void CSeqQueue<T, size>::clear()
{
	_front = 0;
	_rear = 0;
}

template <typename T, int size>
bool CSeqQueue<T, size>::queue(const T &x)
{
	if (is_full())
		return false;

	if (_rear == _max_size) {
		_move_forward();
	}
	_data[_rear++] = x;
	return true;
}

template <typename T, int size>
bool CSeqQueue<T, size>::dequeue(T &x)
{
	if (is_empty())
		return false;

	x = _data[_front++];
	return true;
}

template <typename T, int size>
bool CSeqQueue<T, size>::get_front(T &x) const
{
	if (is_empty())
		return false;

	x = _data[_front];
	return true;
}

template <typename T, int size>
bool CSeqQueue<T, size>::get_back(T &x) const
{
	if (is_empty())
		return false;

	x = _data[_rear - 1];
	return true;
}

template <typename T, int size>
void CSeqQueue<T, size>::show() const
{
	cout << "{ ";
	for (int i = _front; i < _rear; i++) {
		cout << _data[i] << " ";
	}
	cout << "}" << endl;
}


template <typename T, int size>
void CSeqQueue<T, size>::_move_forward()
{
	int index = 0;
	for (int i = _front; i < _rear; i++) {
		_data[index++] = _data[i];
	}
	_front = 0;
	_rear = index;
}
