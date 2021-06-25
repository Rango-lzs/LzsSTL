#ifndef VECTOR_HH
#define VECTOR_HH
#include <vector>
#include "iterator.h"

//assign_dispatch ��ʾ�����ķַ�  ���������� 
//aux   ��ʾ��������

template<class T, class Alloc = alloc>
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
		typedef reverse_iterator<const_iterator>  const_reverse_iterator;
		typedef reverse_iterator<iterator>  reverse_iterator;

	public:
		typedef allocator<T, Alloc>  data_allocator;

    private:
        T* start;
        T* finish;
        T* end_of_storage; // capacity

    public:

		//�������Ǳ�׼��д����������� ʲôʱ����const ʲôʱ�������� 
        // constructor
        vector():start(nullptr) ,end(nullptr),end_of_storage(nullptr){ }

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
		const_iterator cbegin() const {return start}
		iterator end(){return finish;}
		const_iterator cend() const{return finish;}
		reverse_iterator rbegin(){ return reverse_iterator(end());}
		const_reverse_iterator crbegin() const{return const_reverse_iterator(end());}
		reverse_iterator rend(){return reverse_iterator(begin());}
		const_reverse_iterator crend() const{return const_reverse_iterator(begin());}

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
		void insert(iterator pos, size_t n, const T& x);
		template<class InputIterator>
		void insert(InputIterator first, InputIterator last);

		void resize(size_type new_size){ resize(new_size,T());}
		void resize(size_type new_size, const T& x);

		void reserve(size_type n);

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


};



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

template<class T, class Alloc /*= alloc*/>
vector<T, Alloc>::vector(const vector& vec)
{	
	__allocate_and_copy(vec.begin(), vec.end());
}

template<class T, class Alloc /*= alloc*/>
vector<T, Alloc>::vector(const vector&& vec)
{
	start = vec.start;
	finish = vec.finish;
	end_of_storage = vec.end_of_storage;
	vec.start = vec.finish = vec.end_of_storage=nullptr;
}

// ��һ���Ѿ����ڵĶ��󴴽���һ��(������)����  �������캯��
// ��һ���Ѿ����ڵĶ��󴴸�ֵ��һ��(�Ѵ���)����  ��ֵ���캯��
// ǰ��ֻ�踴����Դ���Լ���������Ҫ�������ɵ���Դ

template<class T, class Alloc /*= alloc*/>
vector& vector<T, Alloc>::operator=(const vector& x)
{
	if ((*this) != x)
	{
		const size_t xlen = x.size();
		if (xlen>capacity()) //������������
		{
			__destroy_and_deallocate();
			__allocate_and_copy(x.begin(), x.end());
		}
		else if (size >= xlen)  //С���߼�����
		{
			iterator i = :copy(x.begin(), e.end(), begin());
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
vector<T,Alloc>& vector<T,Alloc>::operator =(vector<T,Alloc>&&x)
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

template<class T, class Alloc /*= alloc*/>
void vector<T, Alloc>::push_back(const T& value)
{
	// β�����һ��Ԫ��
	// �������������� 
	if (finish != end_of_storage) //
	{
		data_allocator::construct(finish, x);
	}
	else
	{
		__insert_aux(end(), x);
	}
}


template<class T, class Alloc /*= alloc*/>
void vector<T, Alloc>::__insert_aux(iterator pos, const T& value)
{
	// ��������ʱ�Ĳ���
	const size_type old_size = size();
	const size_type len = old_size != 0 �� 2 * old_size: 1;
	iterator new_start = data_allocator::allocate(len);
	iterator new_finish = new_start;
	try
	{
		new_finish = uninitialized_copy(start, pos, new_start);
		data_allocator::construct(new_finish, x);
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
	

#endif
