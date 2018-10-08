#pragma once
#include "queue.h"


template <typename T, int size>
class CCircleQueue : public CQueue<T>
{
public:
	CCircleQueue();
	virtual ~CCircleQueue();

	// CQueue
	virtual int capacity() const;
	virtual int length() const;
	virtual bool is_empty() const;
	virtual bool is_full() const;
	virtual void clear();
	virtual bool queue(const T &x);
	virtual bool dequeue(T &x);
	virtual bool get_front(T &x) const;
	virtual bool get_back(T &x) const;
	virtual void show() const;

protected:
	T _data[size];
	int _max_size;
	int _front;
	int _rear;
};
