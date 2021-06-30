#ifndef VECTOR_HH
#define VECTOR_HH
//#include <vector>
#include "iterator.h"
#include "allocator.h"
#include "type_traits.h"
#include "uninitailized.h"
#include "alloc.h"

//assign_dispatch ��ʾ�����ķַ�  ���������� 
//aux   ��ʾ��������

template<class T, class Alloc = alloc<T>>
class vector
{
    public:
		//nested type define
		typedef T				value_type;
		typedef Alloc			allocator_type;
		typedef T*				pointer;
		typedef const T*		const_pointer;
		typedef T&				reference;
		typedef const T&		const_reference;
		typedef size_t			size_type;
		typedef ptrdiff_t		difference_type;
		
		//iterator types
		typedef T* iterator;
		typedef const T* const_iterator;
		//typedef reverse_iterator<const_iterator>  const_reverse_iterator;
		//typedef reverse_iterator<iterator>  reverse_iterator;

	public:
		typedef allocator<T,Alloc>  data_allocator;

    private:
        T* start;
        T* finish;
        T* end_of_storage; // capacity

    public:

		//�������Ǳ�׼��д����������� ʲôʱ����const ʲôʱ�������� 
        // constructor
        vector():start(nullptr) ,finish(nullptr),end_of_storage(nullptr){ }

		explicit vector(size_t n) { __allocate_and_fill(n, T()); }
		vector(size_t n, const T& value) { __allocate_and_fill(n, value); }
		
		template<class InputIterator>
		vector(InputIterator first, InputIterator last);

		// copy constructor
		vector(const vector& vec);
		vector(vector&& vec);

		// copy assignment operator
		//vector<T, Alloc> operator= (const vector<T, Alloc>& other); 
		vector& operator = (const vector& other);

		// ����Ϊ�η��ص������ã���
		vector& operator = (vector&& other);

		//destructor
		~vector(){ __destroy_and_deallocate();}

	//main method
	public:
		//iterator 
		iterator begin() { return start; }
		const_iterator begin() const { return start; }
		iterator end(){return finish;}
		const_iterator end() const{return finish;}

		/*reverse_iterator rbegin(){ return reverse_iterator(end());}
		const_reverse_iterator crbegin() const{return const_reverse_iterator(end());}
		reverse_iterator rend(){return reverse_iterator(begin());}
		const_reverse_iterator crend() const{return const_reverse_iterator(begin());}*/

		//capacity
		//����ֱ��ʹ��start����ʹ��begin()??
		bool empty() const{return begin()==end();}
		size_type size() const{return size_type(end()-begin());}
		size_type max_size() const{return size_type(-1)/sizeof(T);}
		size_type capacity() const { return size_type(end_of_storage-start); }

		void reserve(size_type n);
		void shrink_to_fit();
		
		//element access
		reference operator[] (size_type idx){return*(begin()+n);}
		const_reference operator[] (size_t idx) const { return *(begin()+n);}

		reference at(size_type idx){return (*this)[n];}
		const_reference at(size_type idx) const { return (*this)[n]; }
		
		reference front(){ return return *begin();}
		const_reference front()const { return return *begin(); }
		
		reference back(){ return *end();}
		const_reference back() const { return *end(); }

		size_type data(const T&);

		//elements modifiers
		void assign(size_type poos){ __fill_assign(pos, T());}
		void assign(size_type pos, const T& value){ __fill_assign(pos, value);}
		
		template<class InputIterator>
		void assign(InputIterator first, InputIterator last);

		
		void push_back(const T& value);
		void pop_back();

		iterator erase(iterator pos);
		iterator erase(iterator first, iterator last);
		
		void clear() { erase(begin(), end()); }

		iterator insert(iterator pos);
		iterator insert(iterator pos, const T& x);
		
		void insert(iterator pos, size_type n, const T& x);
		
		template<class InputIterator>
		void insert(iterator pos,InputIterator first, InputIterator last);

		void resize(size_type new_size){ resize(new_size,T());}
		void resize(size_type new_size, const T& x);

		void swap(vector& x);

		allocator_type get_allocator() { return allocator_type(); }
	// for inner use
	private:
		// ���� ���ͽ�������
		template<class Integer>
		void __vector_construct(Integer n, Integer value, __true_type);
		template<class InputIte>
		void __vector_construct(InputIte first, InputIte last, __false_type);
		void __insert_aux(iterator pos, const T& value);

		void __allocate_and_fill(size_t n, const T& value);
		
		void __fill_assign();
		
		void __fill_assign(size_type n, const T& value);

		void __fill_insert(iterator pos, size_type n, const T& x);

		template<class Integer>
		void __insert_dispatch(iterator pos, Integer n, Integer x, __true_type);

		template<class InputIterator>
		void __insert_dispatch(iterator pos, InputIterator first, InputIterator last, __false_type);
		
		void __destroy_and_deallocate();


		template <class Integer>
		void __assign_dispatch(Integer n, Integer value, __true_type);

		template <class InputIterator>
		void __assign_dispatch(InputIterator first, InputIterator last, __false_type);

		template <class InputIterator>
		void __assign_aux(InputIterator first, InputIterator last, input_iterator_tag);
		template <class ForwardIterator>
		void __assign_aux(ForwardIterator first, ForwardIterator last, forward_iterator_tag);
		
		template<class InputIterator>
		void __allocate_and_copy(InputIterator first, InputIterator last);

		template <class InputIterator>
		void __range_insert(iterator position, InputIterator first, InputIterator last, input_iterator_tag);
};

//***************************************************************************************

// ���캯��
template<class T, class Alloc>
template<class InputIterator>
vector<T, Alloc>::vector(InputIterator first, InputIterator last)
{
	typedef typename __is_integer<InputIterator>::is_integer integer;
	__vector_construct(first, last, integer());
}

//���ƹ��캯��
template<class T, class Alloc /*= alloc*/>
vector<T, Alloc>::vector(const vector& vec)
{
	__allocate_and_copy(vec.begin(), vec.end());
}

template<class T, class Alloc /*= alloc*/>
vector<T, Alloc>::vector(vector&& vec)
{
	start = vec.start;
	finish = vec.finish;
	end_of_storage = vec.end_of_storage;
	vec.start = vec.finish = vec.end_of_storage = nullptr;
}

// ��һ���Ѿ����ڵĶ��󴴽���һ��(������)����  �������캯��
// ��һ���Ѿ����ڵĶ��󴴸�ֵ��һ��(�Ѵ���)����  ��ֵ���캯��
// ǰ��ֻ�踴����Դ���Լ���������Ҫ�������ɵ���Դ

template<class T, class Alloc /*= alloc*/>
vector<T, Alloc>& vector<T, Alloc>::operator=(const vector& x)
{
	if ((*this) != x)
	{
		const size_t xlen = x.size();
		if (xlen > capacity()) //������������
		{
			__destroy_and_deallocate();
			__allocate_and_copy(x.begin(), x.end());
		}
		else if (size >= xlen)  //С���߼�����
		{
			iterator i = :copy(x.begin(), x.end(), begin());
			data_allocator::destroy(i, finish);
			finish = start + xlen;
		}
		else
		{
			::copy(x.begin(), x.begin() + size(), start);
			::uninitialized_copy(x.begin() + size(), x.end(), finish);
			end_of_storage = finish = start + xlen;
		}

	}
	return *this;
}

template<class T, class Alloc /* = alloc*/>
vector<T, Alloc>& vector<T, Alloc>::operator =(vector<T, Alloc>&&x)
{
	// �ƶ�һ���ڴ�Ϳ��� ����Ҫ���·���
	if (this != &x)
	{
		__destroy_and_deallocate();
		start = x.start;
		finish = x.finish;
		end_of_storage = x.end_of_storage;
		// ���Ȳ��� ������������
		x.start = x.finish = x.end_of_storage = 0;
	}
	return *this;
}

//��ʼ������
template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::assign(InputIterator first, InputIterator last) {
	typedef typename __is_integer<InputIterator>::is_integer integer;
	__assign_dispatch(first, last, integer());
}

template<class T, class Alloc /*= alloc*/>
void vector<T, Alloc>::push_back(const T& value)
{
	// β�����һ��Ԫ��
	// �������������� 
	if (finish != end_of_storage) //
	{
		data_allocator::construct(finish, value);
		++finish;
	}
	else
	{
		__insert_aux(end(), value);
	}
}

template<class T, class Alloc>
void vector<T, Alloc>::pop_back()
{
	--finish;
	data_allocator::destroy(finish);
}

template<class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::erase(iterator pos)
{
	if (pos + 1 != end())
		::copy(pos + 1, finish, position);
	--finish;
	data_allocator::destroy(finish);
	return pos;
}

template<class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::erase(iterator first, iterator last)
{
	iterator i = ::copy(last, finish, first);
	data_allocator::destroy(i, finish);
	return first;
}

template<class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::insert(iterator pos, const T& x)
{
	size_type n = pos - begin();
	if (finish != end_of_storage && pos == end())
	{
		data_allocator::construct(finish, x);
		++finish;
	}
	else
		__insert_aux(pos, x);
	return begin() + n;
}

template<class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::insert(iterator pos)
{
	size_type n = pos - begin();
	if (finish != end_of_storage && pos == end())
	{
		data_allocator::construct(finish);
		++finish;
	}
	else
		__insert_aux(pos, T());
	return begin() + n;
}

template<class T, class Alloc>
void vector<T, Alloc>::insert(iterator pos, size_type n, const T& value)
{
	__fill_insert(pos, n, x);
}

template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::insert(iterator pos, InputIterator first, InputIterator last)
{
	typedef typename __is_integer<InputIterator>::is_integer integer;
	__insert_dispatch(pos, first, last, integer());
}

template<class T, class Alloc>
void vector<T, Alloc>::resize(size_type new_size, const T&x)
{
	if (new_size < size())
		erase(begin() + new_size, end());
	else
		insert(end(), new_size - size(), x);
}

template<class T, class Alloc>
void vector<T, Alloc>::reserve(size_type n)
{
	if (capacity() < n)
	{
		const size_type old_size = size();
		iterator tmp = data_allocator::allocate(n);
		::uninitialized_copy(start, finish, tmp);
		__destroy_and_deallocate();
		start = tmp;
		finish = tmp + old_size;
		end_of_storage = start + n;
	}
}

template<class T, class Alloc>
void vector<T, Alloc>::swap(vector<T, Alloc>&x)
{
	if (*this != x)
	{
		::swap(start, x.start);
		::swap(finish, x.finish);
		::swap(end_of_storage, x.end_of_storage);
	}
}

template<class T, class Alloc>
template<class Integer>
void vector<T, Alloc>::__vector_construct(Integer n, Integer value, __true_type)
{
	__allocate_and_fill(n, value);
}

template<class T, class Alloc>
template<class InputIte>
void vector<T, Alloc>::__vector_construct(InputIte first, InputIte last, __false_type)
{
	__allocate_and_copy(n, value);
}

template<class T,class Alloc>
void vector<T, Alloc>::__destroy_and_deallocate()
{
	data_allocator::destroy(start, finish);
	data_allocator::deallocate(start, end_of_storage - start);
}

template<class T, class Alloc>
void vector<T, Alloc>::__allocate_and_fill(size_t n, const T& value)
{
	start = data_allocator::allocate(n);
	finish = ::uninitialized_fill_n(begin(), n, value);
	end_of_storage = finish;
}

template<class T,class Alloc>
template<class InputIterator>
void vector<T, Alloc>::__allocate_and_copy(InputIterator first, InputIterator last)
{
	difference_type n = last - first;
	start = data_allocator::allocate(n);
	finish = ::uninitialized_copy(first, last, start);
	end_of_storage = finish;
}

template<class T, class Alloc>
void vector<T, Alloc>::__fill_assign(size_type n, const T& value)
{
	if (n > capacity())
	{
		vector<T, Alloc> tmp(n, value);
		tmp.swap(*this);
	}
	else if (n > size)
	{
		::fill_n(begin(), end(), value);
		finish = uninitialized_fill_n(finish, n - size(), value);
	}
	else
	{
		erase(::fill_n(start, n, value), finish);
	}
}

template<class T, class Alloc>
template<class Integer>
void vector<T, Alloc>::__assign_dispatch(Integer n, Integer value, __true_type)
{
	__fill_assign(n, value);
}

template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::__assign_dispatch(InputIterator first, InputIterator last, __false_type)
{
	__assign_aux(first,last, ::iteratory_category(first));
}

//not POD type??
template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::__assign_aux(InputIterator first, InputIterator last, input_iterator_tag)
{
	iterator cur = begin();
	for (; first != last && cur != end(); ++first, ++cur)
	{
		*cur = *first;
	}
	if (first == last)
		erase(cur, end());
	else
		insert(end(), first, last);
}

// POD type?? 
template<class T, class Alloc>
template<class ForwardIterator>
void vector<T, Alloc>::__assign_aux(ForwardIterator first, ForwardIterator last, forward_iterator_tag)
{
	size_type len = distance(first, last);
	if (len > capacity())
	{
		__destroy_and_deallocate();
		__allocate_and_copy(first, last);
	}
	else if (size()>=len)
	{
		iterator new_finish = ::copy(first, last, start);
		data_allocator::destroy(new_finish, finish);
		finish = new_finish;
	}
	else
	{
		// ǰ�벿��ֱ��copy  ��벿��δ��ʼ��copy
		ForwardIterator mid = first;
		advance(mid, size());
		::copy(first, mid, start);
		finish = ::uninitialized_copy(mid, last, finish);
	}
}


template<class T, class Alloc /*= alloc*/>
void vector<T, Alloc>::__insert_aux(iterator pos, const T& value)
{
	// ��������ʱ�Ĳ���
	const size_type old_size = size();
	const size_type len = old_size != 0 ? 2 * old_size : 1;
	iterator new_start = data_allocator::allocate(len);
	iterator new_finish = new_start;
	try
	{
		new_finish = uninitialized_copy(start, pos, new_start);
		data_allocator::construct(new_finish, value);
		++new_finish;
		new_finish = uninitialized_copy(pos, finish, new_finish);
	}
	catch (...)
	{
		data_allocator::destroy(new_start, new_finish);
		data_allocator::deallocate(new_start, len);
		throw;
	}
	__destroy_and_deallocate();
	start = new_start;
	finish = new_finish;
	end_of_storage = new_start + len;
}

template<class T, class Alloc>
template<class Integer>
void vector<T, Alloc>::__insert_dispatch(iterator pos, Integer n, Integer x, __true_type)
{
	__fill_insert(pos, n, x);
}

template<class T, class Alloc>
template<class InputIterator>
void vector<T, Alloc>::__insert_dispatch(iterator pos, InputIterator first, InputIterator last, __false_type)
{
	__range_insert(pos, first, last, ::iteratory_category(first));
}

template<class T, class Alloc>
void vector<T, Alloc>::__fill_insert(iterator pos, size_type n, const T& x)
{
	if (n != 0)
	{

	}
}

template<class T, class Alloc>
template <class InputIterator>
void vector<T, Alloc>::__range_insert(iterator position, InputIterator first, InputIterator last, input_iterator_tag)
{
	for (; first != last; ++first)
	{
		position = insert(position, *first);
		++position;
	}
}


// ���ز�����

template<class T,class Alloc>
inline bool operator == (const vector<T, Alloc> &x, const vector<T, Alloc>& y)
{
	return x.size() == y.size() && ::equal(x, begin(), x.end(), y.begin(), y.end());
}



#endif

/*
vector[ * * * * .)   [begin, end)
*/

/*
assign -> ��ֵ����   

1. assign(n,value) ֱ�Ӹ�ֵ
	
	__fill_assign(n,value)
		unitialized_fill_n | fill_n

2 assign(class first, class last) ͨ����������Χ��ֵ ���ݵ������Ƿ�Ϊinteger() ��������

	__assign_dispatch(first ,last ,integer())
		
		__assign_dispatch(first ,last ,_true_type)
			__fill_assign(n,value)

		__assign_dispatch(first ,last ,_false_type)
			__assign_aux(first, last, category())

*/

/*
insert -> �������

1. ֱ����ĳ��λ��(iterator pos)����һ������Ԫ��
	һ��Ԫ�أ���ĩβ����ֱ�� construct ������ __insert_aux �������洦�����ݵ��߼�
	���Ԫ��: __fill_insert


2 �� pos λ�ò���[first, last)�ڵ�Ԫ��

	__insert_dispatch(integer())
		__true_type: __fill_insert()

		__false_type:__range_insert()
			forward_ite: uninitialized_copy
			input_ite: insert one by one 
*/


/*
����������Ƿ���POD type �������أ� POD type ����ֱ��memcpy, ��POD ��Ҫ���ù��������и��ơ�

*/



/*
��Ҫ��������⣺
1.�ڴ�ķ���͹���  
2.vector����һЩ�㷨  ��ɾ�Ĳ��һЩ�㷨
3.������ƣ� ���������
*/