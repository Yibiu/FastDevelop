#pragma once
#include <stdint.h>


template <typename T>
struct link_node_t
{
	T data;
	link_node_t<T> *next;
	link_node_t<T> *pre; // Used for double list
};

