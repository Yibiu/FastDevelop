#include "double_list.h"


template <typename T>
CDoubleList<T>::CDoubleList()
{
	_head = new link_node_t<T>;
	_head->next = _head;
	_head->pre = _head;
}

template <typename T>
CDoubleList<T>::~CDoubleList()
{
	delete _head;
}

template <typename T>
int CDoubleList<T>::capacity() const
{
	return length();
}

template <typename T>
int CDoubleList<T>::length() const
{
	int count = 0;
	link_node_t<T> *cur_ptr = _head->next;
	while (_head != cur_ptr)
	{
		count++;
		cur_ptr = cur_ptr->next;
	}

	return count;
}

template <typename T>
bool CDoubleList<T>::is_empty() const
{
	return (_head == _head->next);
}

template <typename T>
bool CDoubleList<T>::is_full() const
{
	return false;
}

template <typename T>
void CDoubleList<T>::clear()
{
	link_node_t<T> *cur_ptr = _head->next;
	while (_head != cur_ptr)
	{
		link_node_t<T> *tmp = cur_ptr;
		cur_ptr = cur_ptr->next;
		delete tmp;
	}
	_head->next = _head;
	_head->pre = _head;
}

template <typename T>
bool CDoubleList<T>::insert(int index, const T &x)
{
	if (index < 0 || index > length())
		return false;

	int count = 0;
	link_node_t<T> *pre_ptr = _head;
	link_node_t<T> *cur_ptr = _head->next;
	while (count < index)
	{
		pre_ptr = cur_ptr;
		cur_ptr = cur_ptr->next;
		count++;
	}
	link_node_t<T> *new_ptr = new link_node_t<T>;
	new_ptr->data = x;
	new_ptr->next = cur_ptr;
	new_ptr->pre = pre_ptr;
	pre_ptr->next = new_ptr;
	cur_ptr->pre = new_ptr;

	return true;
}

template <typename T>
bool CDoubleList<T>::remove(int index)
{
	if (index < 0 || index >= length())
		return false;

	int count = 0;
	link_node_t<T> *pre_ptr = _head;
	link_node_t<T> *cur_ptr = _head->next;
	while (count < index)
	{
		pre_ptr = cur_ptr;
		cur_ptr = cur_ptr->next;
		count++;
	}
	pre_ptr->next = cur_ptr->next;
	pre_ptr->next->pre = pre_ptr;
	delete cur_ptr;

	return true;
}

template <typename T>
int CDoubleList<T>::find(const T &x) const
{
	int index = -1;
	link_node_t<T> *cur_ptr = _head->next;
	while (_head != cur_ptr)
	{
		index++;
		if (cur_ptr->data == x) {
			break;
		}
	}

	return index;
}

template <typename T>
bool CDoubleList<T>::get(int index, T &x) const
{
	if (index < 0 || index >= length())
		return false;

	int count = 0;
	link_node_t<T> *cur_ptr = _head->next;
	while (count < index)
	{
		cur_ptr = cur_ptr->next;
		count++;
	}
	x = cur_ptr->data;

	return true;
}

template <typename T>
bool CDoubleList<T>::set(int index, const T &x)
{
	if (index < 0 || index >= length())
		return false;

	int count = 0;
	link_node_t<T> *cur_ptr = _head->next;
	while (count < index)
	{
		cur_ptr = cur_ptr->next;
		count++;
	}
	cur_ptr->data = x;

	return true;
}

template <typename T>
void CDoubleList<T>::show() const
{
	cout << "{ ";
	link_node_t<T> *cur_ptr = _head->next;
	while (_head != cur_ptr)
	{
		cout << cur_ptr->data << " ";
		cur_ptr = cur_ptr->next;
	}
	cout << "}" << endl;
}

template <typename T>
link_node_t<T>* CDoubleList<T>::get_head()
{
	return _head;
}
