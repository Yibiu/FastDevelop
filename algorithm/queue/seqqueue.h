#pragma once
#include "queue.h"


template <typename T, int size>
class CSeqQueue : public CQueue<T>
{
public:
	CSeqQueue();
	virtual ~CSeqQueue();

	// CQueue
	virtual int capacity() const;
	virtual int length() const;
	virtual bool is_empty() const;
	virtual bool is_full() const;
	virtual bool queue(const T &x);
	virtual bool dequeue(T &x);
	virtual bool get_front(T &x) const;
	virtual bool get_back(T &x) const;
	virtual void show() const;

protected:
	void _move_forward();

protected:
	T _data[size];
	int _max_size;
	int _front;
	int _rear;
};