#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
using namespace std;


template <typename T>
struct link_node_t
{
	T data;
	link_node_t<T> *next;
	link_node_t<T> *pre; // Used for double list
};


template <typename T>
class CLinear
{
public:
	CLinear() {}
	virtual ~CLinear() {}

	virtual int capacity() const = 0;
	virtual int length() const = 0;
	virtual bool is_empty() const = 0;
	virtual bool is_full() const = 0;
	virtual void clear() = 0;

	virtual bool insert(int index, const T &x) = 0;
	virtual bool remove(int index) = 0;
	virtual int find(const T &x) const = 0;
	virtual bool get(int index, T &x) const = 0;
	virtual bool set(int index, const T &x) = 0;

	virtual void show() const = 0;
};
