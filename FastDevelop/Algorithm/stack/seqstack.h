#pragma once
#include "stack.h"


template <typename T, int size>
class CSeqStack : public CStack<T>
{
public:
	CSeqStack();
	virtual ~CSeqStack();

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
	T _data[size];
	int _max_size;
	int _top;
};
