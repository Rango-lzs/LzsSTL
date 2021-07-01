#ifndef ALLOCATOR_HH
#define ALLOCATOR_HH

#include "alloc.h"
#include "construct.h"

template <class T,class Alloc=alloc>
class allocator
{
    public:
        typedef T           value_type;
        typedef T*          pointer;
        typedef const T*    const_pointer;
        typedef T&          reference;
        typedef const T&    const_reference;
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;
    public:
        static T* allocate();
        static T* allocate(size_t n);
        static void deallocate(T* ptr);
        static void deallocate(T* ptr, size_t n);

        static void construct(T* ptr);
        static void construct(T* ptr,const T&x);
        static void destroy(T* ptr);
        static void destroy(T* first,T* last);
};

template<class T,class Alloc>
T* allocator<T,Alloc>::allocate()
{
    return static_cast<T*>(Alloc::allocate(sizeof(T)));
}

template<class T, class Alloc>
T* allocator<T,Alloc>::allocate(size_t n)
{
    if(n==0) return nullptr;
    return static_cast<T*>(Alloc::allocate(n*sizeof(T)));
}

template<class T, class Alloc>
void allocator<T,Alloc>::deallocate(T* ptr)
{
    if(ptr==nullptr) return;
	Alloc::deallocate(ptr);
}

template<class T,class Alloc>
void allocator<T,Alloc>::deallocate(T* ptr, size_t n)
{
    if(ptr==nullptr) return;
	Alloc::deallocate(ptr);
}

template<class T,class Alloc>
void allocator<T,Alloc>::construct(T* ptr)
{
    ::construct(ptr);
}

template<class T,class Alloc>
void allocator<T,Alloc>::construct(T* ptr,const T& x)
{
    ::construct(ptr,x);
}

template<class T, class Alloc>
void allocator<T,Alloc>::destroy(T* ptr)
{
    ::destroy(ptr);
}

template<class T, class Alloc>
void allocator<T,Alloc>::destroy(T* first, T* last)
{
    ::destroy(first,last);
}
#endif

/*
alloactor 提供的基本功能：

1. allocate and deallocate 分配内存

	此功能有alloc类来提供


2. construct and destroy 构造和销毁元素

	此功能由全局的construct 来提供
*/
