#pragma once
#include "linear.h"


// Sequence list
template <typename T, int size>
class CSeqLinear : public CLinear<T>
{
public:
	CSeqLinear();
	virtual ~CSeqLinear();

	// CLinear
	virtual int capacity() const;
	virtual int length() const;
	virtual bool is_empty() const;
	virtual bool is_full() const;
	virtual void clear();
	virtual bool insert(int index, const T &x);
	virtual bool remove(int index);
	virtual int find(const T &x) const;
	virtual bool get(int index, T &x) const;
	virtual bool set(int index, const T &x);
	virtual void show() const;

	T& operator[](int index);

protected:
	T _data[size];
	int _max_size;
	int _last;
};
