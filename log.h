#pragma once
#include <stdio.h>


#define LOG(level, fmt, ...)	printf("["#level"]: "#fmt"\n", ##__VA_ARGS__)
