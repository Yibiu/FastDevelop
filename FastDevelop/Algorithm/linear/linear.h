#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
using namespace std;


/**
* @brief:
* Basic class for linear list
*
* 线性表(Linear List) = 顺序表(Sequential List) + 链表(Linked List)
* 顺序表：基于数组，内存连续
*	1. 静态方式：事先分配，固定内存大小
*	2. 动态方式：使用时分配，动态扩展内存大小
* 链表：基于指针和动态分配，内存不连续
*/
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
