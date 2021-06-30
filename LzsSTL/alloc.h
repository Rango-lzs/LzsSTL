#ifndef _ALLOC_HH
#define _ALLOC_HH
#include <stdlib.h>
template<class T>
class alloc
{
public:
	static T* allocate(size_t n) { 
		//return (T*)malloc(n*sizeof(T)); 
		//return new T[n];
		return (T*)operator new(n);
	}
	static void deallocate(T* ptr) { free(ptr); }
};

#endif
