#ifndef CONSTRUCT_HH
#define CONSTRUCT_HH

#incldue "type_traits.h"
#incldue "iterator.h"

//构造函数,  分配内存   在分配的内存上构造对象

template <class T1>
inline void construct(T1* p)
{
	new ((void*)p) T1();
}
template <class T1, class T2>
inline void construct(T1 *p, const T2& value)
{
	new ((void*)p) T1(value);
}

template<class T>
inline void destroy(T* pointer)
{
	pointer->~T();
}

template <class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last)
{
	//value_type is a function in iterator.h
	__destroy(first, last, value_type(first));
}

template <class ForwardIterator, class T>
inline void __destroy(ForwardIterator first, ForwardIterator last, T*)
{
	typedef typename __type_traits<T>::has_trivial_destructor trival_destructor;
	return __destroy_aux(first, last, trival_destructor());
}

template<class ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __false_type)
{
	for (; first != last; ++first)
		destroy(first);  //destroy(&*first) why
}

template<class ForwardIterator>
inline void __destroy_aux(ForwardIterator first, ForwardIterator last, __true_type)
{
	
}

template<>
inline void destroy(char*, char*){}

template<>
inline void destroy(wchar_t* , wchar_t*){}

#endif 