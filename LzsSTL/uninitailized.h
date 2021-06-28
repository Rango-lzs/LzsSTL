#ifndef UNINITIALIZED_HH
#define UNINITIALIZED_HH

// δ��ʼ�������� copy fill fill_n����


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

//�����ǲ�֧��ƫ�ػ���
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
// Ϊʲô��ֱ��ͨ��T ����ȡ�Ƿ���pod ���� ������Ϊֻ�е������Ŷ�����is_pod??
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
// Ϊʲô��ֱ��ͨ��T ����ȡ�Ƿ���pod ���� ������Ϊֻ�е������Ŷ�����is_pod??
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