#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
using namespace std;


template <typename T>
class CSort
{
public:
	CSort();
	virtual ~CSort();

	virtual void bubble(T *arrays, int n);
	virtual void insert(T *arrays, int n);
	virtual void select(T *arrays, int n);
	virtual void shell(T *arrays, int n);

	virtual void show(T *arrays, int n);

protected:
	void _swap(T &a, T &b);
};