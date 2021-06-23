#ifndef _ALLOC_HH
#define _ALLOC_HH

template<class T>
class alloc
{
    static T* allocate(size_t n);
    static void deallocate(T* ptr);
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
