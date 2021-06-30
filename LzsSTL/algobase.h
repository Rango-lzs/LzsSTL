#ifndef ALGOBASE_H
#define ALGOBASE_H

#ifndef USE_CSTRING
#define USE_CSTRING
#include <cstring>
#endif // !USE_CSTRING

#include "iterator.h"
#include "type_traits.h"

	/*********************************************************************************/
	// copy
	// 把 [first, last)区间内的元素复制到 [result, result + (last - first))内
	/*********************************************************************************/
	//完全泛化版本
	template <class InputIterator, class OutputIterator>
	inline OutputIterator copy(InputIterator first, InputIterator last,
		OutputIterator result) {
		return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
	}

	//const char*的特化版本
	inline char* copy(const char* first, const char* last, char* result) {
		memmove(result, first, last - first);
		return result + (last - first);
	}

	//const wchar_t*的特化版本
	inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
		memmove(result, first, sizeof(wchar_t) * (last - first));
		return result + (last - first);
	}

	// __copy_dispatch的完全泛化版本
	template <class InputIterator, class OutputIterator>
	struct __copy_dispatch {
		OutputIterator operator()(InputIterator first, InputIterator last,
			OutputIterator result) {
			return __copy(first, last, result, iterator_category(first));
		}
	};

	// __copy_dispatch的偏特化版本(1)，两个参数都是 T* 指针形式
	template <class T>
	struct __copy_dispatch<T*, T*> {
		T* operator()(T* first, T* last, T* result) {
			typedef typename __type_traits<T>::has_trivial_assignment_operator t;
			return __copy_t(first, last, result, t());
		}
	};

	// __copy_dispatch的偏特化版本(2)，第一个参数是 const T* 指针形式，第二个参数是 T* 指针形式
	template <class T>
	struct __copy_dispatch<const T*, T*> {
		T* operator()(const T* first, const T* last, T* result) {
			typedef typename __type_traits<T>::has_trivial_assignment_operator t;
			return __copy_t(first, last, result, t());
		}
	};

	// __copy 的 InputIterator 版本
	template <class InputIterator, class OutputIterator>
	inline OutputIterator __copy(InputIterator first, InputIterator last,
		OutputIterator result, input_iterator_tag) {
		// 判断迭代器是否相等决定循环，速度慢
		for (; first != last; ++first, ++result) {
			*result = *first;
		}
		return result;
	}


	// __copy_d 
	template <class RandomAccessIterator, class OutputIterator, class Distance>
	inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last,
		OutputIterator result, Distance*) {
		//以 n 决定循环次数，速度快
		for (Distance n = last - first; n > 0; --n, ++first, ++result) {
			*result = *first;
		}
		return result;
	}

	// __copy_t : 指针所指对象具备 trivial assignment operator
	template <class T>
	inline T* __copy_t(const T* first, const T* last, T* result, __true_type) {
		memmove(result, first, sizeof(T) * (last - first));
		return result + (last - first);
	}

	// __copy_t : 指针所指对象具备 non-trivial assignment operator
	template <class T>
	inline T* __copy_t(const T* first, const T* last, T* result, __false_type) {
		// 原生指针也是 RandomAccessIterator，交给 __copy_d 处理
		return __copy_d(first, last, result, (ptrdiff_t*)0);
	}

#endif // !ALGOBASE_H
