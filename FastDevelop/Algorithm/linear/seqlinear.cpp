#include "seqlinear.h"


template <typename T, int size>
CSeqLinear<T, size>::CSeqLinear()
{
	_max_size = size;
	_last = -1;
}

template <typename T, int size>
CSeqLinear<T, size>::~CSeqLinear()
{
}

template <typename T, int size>
int CSeqLinear<T, size>::capacity() const
{
	return _max_size;
}

template <typename T, int size>
int CSeqLinear<T, size>::length() const
{
	return (_last + 1);
}

template <typename T, int size>
bool CSeqLinear<T, size>::is_empty() const
{
	return (-1 == _last);
}

template <typename T, int size>
bool CSeqLinear<T, size>::is_full() const
{
	return ((_last + 1) == _max_size);
}

template <typename T, int size>
void CSeqLinear<T, size>::clear()
{
	_last = -1;
}

template <typename T, int size>
bool CSeqLinear<T, size>::insert(int index, const T &x)
{
	if (is_full())
		return false;
	if (index < 0 || index > _last + 1)
		return false;

	for (int i = _last; i >= index; i--) {
		_data[i + 1] = _data[i];
	}
	_data[index] = x;
	_last++;

	return true;
}

template <typename T, int size>
bool CSeqLinear<T, size>::remove(int index)
{
	if (is_empty())
		return false;
	if (index < 0 || index > _last)
		return false;

	for (int i = index; i < _last; i++) {
		_data[i] = _data[i + 1];
	}
	_last--;

	return true;
}

template <typename T, int size>
int CSeqLinear<T, size>::find(const T &x) const
{
	int index = -1;
	for (int i = 0; i <= _last; i++) {
		if (_data[i] == x) {
			index = i;
			break;
		}
	}

	return index;
}

template <typename T, int size>
bool CSeqLinear<T, size>::get(int index, T &x) const
{
	if (index < 0 || index > _last)
		return false;

	x = _data[index];
	return true;
}

template <typename T, int size>
bool CSeqLinear<T, size>::set(int index, const T &x)
{
	if (index < 0 || index > _last)
		return false;

	_data[index] = x;
	return true;
}

template <typename T, int size>
void CSeqLinear<T, size>::show() const
{
	cout << "{ ";
	for (int i = 0; i <= _last; i++) {
		cout << _data[i] << " ";
	}
	cout << "}" << endl;
}

template <typename T, int size>
T& CSeqLinear<T, size>::operator[](int index)
{
	if (index < 0 || index > _last)
		assert(0);

	return _data[index];
}

