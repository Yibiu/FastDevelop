#pragma once
#include "linear.h"


// Single circle list
template <typename T>
class CCircleList : public CLinear<T>
{
public:
	CCircleList();
	virtual ~CCircleList();

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

	link_node_t<T>* get_head();

protected:
	link_node_t<T> *_head;
};