#include "single_list.h"


template <typename T>
CSingleList<T>::CSingleList()
{
	_head = new link_node_t<T>;
	_head->next = NULL;
}

template <typename T>
CSingleList<T>::~CSingleList()
{
	delete _head;
}

template <typename T>
int CSingleList<T>::capacity() const
{
	return length();
}

template <typename T>
int CSingleList<T>::length() const
{
	int count = 0;
	link_node_t<T> *cur_ptr = _head->next;
	while (NULL != cur_ptr)
	{
		count++;
		cur_ptr = cur_ptr->next;
	}

	return count;
}

template <typename T>
bool CSingleList<T>::is_empty() const
{
	return (NULL == _head->next);
}

template <typename T>
bool CSingleList<T>::is_full() const
{
	return false;
}

template <typename T>
void CSingleList<T>::clear()
{
	link_node_t<T> *cur_ptr = _head->next;
	while (NULL != cur_ptr)
	{
		link_node_t<T> *tmp = cur_ptr;
		cur_ptr = cur_ptr->next;
		delete tmp;
	}
	_head->next = NULL;
}

template <typename T>
bool CSingleList<T>::insert(int index, const T &x)
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
	pre_ptr->next = new_ptr;

	return true;
}

template <typename T>
bool CSingleList<T>::remove(int index)
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
	delete cur_ptr;

	return true;
}

template <typename T>
int CSingleList<T>::find(const T &x) const
{
	int index = -1;
	link_node_t<T> *cur_ptr = _head->next;
	while (NULL != cur_ptr)
	{
		index++;
		if (cur_ptr->data == x) {
			break;
		}
	}

	return index;
}

template <typename T>
bool CSingleList<T>::get(int index, T &x) const
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
bool CSingleList<T>::set(int index, const T &x)
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
void CSingleList<T>::show() const
{
	cout << "{ ";
	link_node_t<T> *cur_ptr = _head->next;
	while (NULL != cur_ptr)
	{
		cout << cur_ptr->data << " ";
		cur_ptr = cur_ptr->next;
	}
	cout << "}" << endl;
}

template <typename T>
const link_node_t<T>* CSingleList<T>::get_head()
{
	return _head;
}
