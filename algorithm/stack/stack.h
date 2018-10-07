#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;


template <typename T>
struct link_node_t
{
	T data;
	link_node_t<T> *next;
};


template <typename T>
class CStack
{
public:
	CStack() {}
	virtual ~CStack() {}

	virtual int capacity() const = 0;
	virtual int length() const = 0;
	virtual bool is_empty() const = 0;
	virtual bool is_full() const = 0;
	virtual void clear() = 0;

	virtual bool push(const T &x) = 0;
	virtual bool pop(T &x) = 0;
	virtual bool get_top(T &x) const = 0;

	virtual void show() const = 0;
};
