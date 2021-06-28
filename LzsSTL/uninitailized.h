#ifndef UNINITIALIZED_HH
#define UNINITIALIZED_HH

// 未初始化容器的 copy fill fill_n操作


// iterator ->trits: value_type  copy(f,l,r)->value_type  _copy(f,l,r,value_type)
// value_type ->trats: is_Pod_type  _copy(f,l,r,value_type)->_copy_aux(f,l,r,pod || not_pod)  ,overload 

template<class InputIterator ,class ForwardIterator>
inline ForwardIterator 
uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
{
	return __uninitialized_copy(first, last, result, value_type(result));
}

template<class InputIterator, class ForwardIterator, class T>
inline ForwardIterator
__uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result,T*)
{
	typedef typename __type_traits<T>::is_POD_type POD_type;
	return __uninitialized_copy_aux(first, last, result, POD_type());
}

template<class InputIterator, class ForwardIterator>
inline ForwardIterator
__uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type)
{
	return ::copy(first, last, result);
}

template<class InputIterator, class ForwardIterator>
inline ForwardIterator
__uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type)
{
	for (; first != last; ++first,++result)
	{
		construct(result, *first);
	}
	return result;
}

//函数是不支持偏特化的
inline char* uninitialized_copy(const char* first, const char* last, char* des)
{
	memmove(des, first, sizeof(char)*(last - first));
	return des + (last - first);
}

inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, wchar_t* des)
{
	memmove(des, first, sizeof(wchar_t)*(last - first));
	return des + (last - first);
}


//************  fill  **************
template<class ForwardIterator,class T>
inline void
uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value)
{
	 __uninitialized_fill(first, last, value,value_type(first));
}
// 为什么不直接通过T 来获取是否是pod 类型 ，是因为只有迭代器才定义了is_pod??
template<class ForwardIterator, class T>
inline void
__uninitialized_fill(ForwardIterator first, ForwardIterator last,const T&value, T*)
{
	typedef typename __type_traits<T>::is_POD_type POD_type;
	 __uninitialized_fill_aux(first, last, value, POD_type());
}

template< class ForwardIterator, class T>
inline void
__uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, __true_type)
{
	 ::fill(first, last, value);
}

template<class ForwardIterator>
inline 
__uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, __false_type)
{
	for (; first != last; ++first)
	{
		construct(first, value);
	}
}


//************  fill_n  **************
template<class ForwardIterator, class T>
inline void
uninitialized_fill_n(ForwardIterator first,size_t n, const T& value)
{
	 __uninitialized_fill(first, n, value, value_type(first));
}
// 为什么不直接通过T 来获取是否是pod 类型 ，是因为只有迭代器才定义了is_pod??
template<class ForwardIterator, class T>
inline void
__uninitialized_fill_n(ForwardIterator first,size_t n, const T&value, T*)
{
	typedef typename __type_traits<T>::is_POD_type POD_type;
	 __uninitialized_fill_n_aux(first, n, value, POD_type());
}

template< class ForwardIterator, class T>
inline void
__uninitialized_fill_aux(ForwardIterator first,size_t n, const T& value, __true_type)
{
	 ::fill_n(first, n, value);
}

template<class ForwardIterator>
inline void
__uninitialized_fill_aux(ForwardIterator first, size_t n, const T& value, __false_type)
{
	for (; n>0;--n, ++first)
	{
		construct(first, value);
	}
}

#endif