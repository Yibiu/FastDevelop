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
* ���Ա�(Linear List) = ˳���(Sequential List) + ����(Linked List)
* ˳����������飬�ڴ�����
*	1. ��̬��ʽ�����ȷ��䣬�̶��ڴ��С
*	2. ��̬��ʽ��ʹ��ʱ���䣬��̬��չ�ڴ��С
* ��������ָ��Ͷ�̬���䣬�ڴ治����
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
