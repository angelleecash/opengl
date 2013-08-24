#ifndef __UTIL_C__
#define __UTIL_C__

#include <stdio.h>

#define VERBOSE 1
#define EMPTY printf("%s", "")
#define VERBOSE_HEADER ""

#define STACK_DUMP(L) (VERBOSE ? (stack_dump(L)) : (EMPTY))

#ifdef WIN32

#include "windows.h"
extern char _utilBuffer[1024];

#define P(format, ...) \
	{\
		sprintf_s(_utilBuffer,format,__VA_ARGS__);\
		OutputDebugString((_utilBuffer));\
	}
#else
#define P(format, ...) (VERBOSE ? fprintf(stderr, VERBOSE_HEADER format , ## __VA_ARGS__) : (EMPTY))
#endif

inline int Align(int value, int base)
{
	int newValue = value + (base - (value%base)) %base;

	//ASSERT((newValue % base) == 0, "Align fail");
	return newValue;
}

void Dump(char* p, int n);

#endif
