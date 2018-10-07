#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;


template <typename T>
class CQueue
{
public:
	CQueue() {}
	virtual ~CQueue() {}

	virtual int capacity() const = 0;
	virtual int length() const = 0;
	virtual bool is_empty() const = 0;
	virtual bool is_full() const = 0;

	virtual bool queue(const T &x) = 0;
	virtual bool dequeue(T &x) = 0;
	virtual bool get_front(T &x) const = 0;
	virtual bool get_back(T &x) const = 0;

	virtual void show() const = 0;
};
