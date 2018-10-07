#include "list_stack.h"


template <typename T>
CListStack<T>::CListStack()
{
	_head = NULL;
}

template <typename T>
CListStack<T>::~CListStack()
{
}

template <typename T>
int CListStack<T>::capacity() const
{
	return length();
}

template <typename T>
int CListStack<T>::length() const
{
	int count = 0;
	link_node_t<T> *cur_ptr = _head;
	while (NULL != cur_ptr)
	{
		cur_ptr = cur_ptr->next;
		count++;
	}

	return count;
}

template <typename T>
bool CListStack<T>::is_empty() const
{
	return (NULL == _head);
}

template <typename T>
bool CListStack<T>::is_full() const
{
	return false;
}

template <typename T>
void CListStack<T>::clear()
{
	link_node_t<T> *cur_ptr = _head;
	while (NULL != cur_ptr)
	{
		link_node_t<T> *tmp = cur_ptr;
		cur_ptr = cur_ptr->next;
		delete tmp;
	}
	_head = NULL;
}

template <typename T>
bool CListStack<T>::push(const T &x)
{
	link_node_t<T> *new_ptr = new link_node_t<T>;
	if (NULL == new_ptr)
		return false;

	new_ptr->data = x;
	new_ptr->next = _head;
	_head = new_ptr;
	return true;
}

template <typename T>
bool CListStack<T>::pop(T &x)
{
	if (is_empty())
		return false;

	x = _head->data;
	link_node_t<T> *tmp = _head;
	_head = _head->next;
	delete tmp;
	return true;
}

template <typename T>
bool CListStack<T>::get_top(T &x) const
{
	if (is_empty())
		return false;

	x = _head->data;
	return true;
}

template <typename T>
void CListStack<T>::show() const
{
	link_node_t<T> *cur_ptr = _head;
	cout << "{ ";
	while (NULL != cur_ptr)
	{
		cout << cur_ptr->data << " ";
		cur_ptr = cur_ptr->next;
	}
	cout << "}" << endl;
}

