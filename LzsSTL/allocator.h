#ifndef ALLOCATOR_HH
#define ALLOCATOR_HH

#include "alloc.h"
//#include "construct.h"

template <class T>
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

template<class T>
T* allocate<T>::allocate()
{
    return static_cast<T*>(alloc::allocate(sizeof(T)));
}

template<class T>
T* allocate<T>::allocate(size_t n)
{
    if(n==0) return nullptr;
    return static_cast<T*>(alloc::allocate(n*sizeof(T)));
}

template<class T>
void allocate<T>::deallocate(T* ptr)
{
    if(ptr==nullptr) return;
    alloc::deallocate(ptr,sizeof(T));
}

template<class T>
void allocate<T>::deallocate(T* ptr, size_t n)
{
    if(ptr==nullptr) return;
    alloc::deallocate(ptr, n*sizeof(T));
}

template<class T>
void allocate<T>::construct(T* ptr)
{
    ::construct(ptr);
}

template<class T>
void allocate<T>::construct(T* ptr,const T& x)
{
    ::construct(ptr,x);
}

template<class T>
void allocate<T>::destroy(T* ptr)
{
    ::destroy(ptr);
}

template<class T>
void allocate<T>::destroy(T* first, T* last)
{
    ::destroy(first,last);
}
#endif
