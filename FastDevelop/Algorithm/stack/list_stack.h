#pragma once
#include "../defines.h"
#include "stack.h"


template <typename T>
class CListStack : CStack<T>
{
public:
	CListStack();
	virtual ~CListStack();

	// CStack
	virtual int capacity() const;
	virtual int length() const;
	virtual bool is_empty() const;
	virtual bool is_full() const;
	virtual void clear();
	virtual bool push(const T &x);
	virtual bool pop(T &x);
	virtual bool get_top(T &x) const;
	virtual void show() const;

protected:
	link_node_t<T> *_head;
};
