#ifndef _ALLOC_HH
#define _ALLOC_HH
#include <stdlib.h>
template<class T>
class alloc
{
public:
	static T* allocate(size_t n) { 
		//return (T*)malloc(n*sizeof(T)); 
		return new T[n];
	}
	static void deallocate(T* ptr) { free(ptr); }
};

template<class T>
void construct(T*ptr)
{
}

template<class T>
void construct(T* ptr, const T& x)
{
}

template<class T>
void destroy(T* ptr)
{
}

template<class T>
void destroy(T* first, T* last)
{
}
#endif
