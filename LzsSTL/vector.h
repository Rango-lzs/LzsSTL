#ifndef VECTOR_HH
#define VECTOR_HH
#include <vector>
#include "iterator.h"

//assign_dispatch 表示方法的分发  即方法重载 
//aux   表示辅助函数

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

		//基本都是标准的写法，熟记于心 什么时候是const 什么时候是引用 
        // constructor
        vector():start(nullptr) ,end(nullptr),end_of_storage(nullptr){ }
		explicit vector(size_t n) { __allocate_and_fill(n, T()); }
		vector(size_t n, const T& value) { __allocate_and_fill(n, value); }
		
		template<class InputIterator>
		vector(InputIterator first, InputIterator last);

		// copy constructor
		vector(const vector& vec);
		vector(vector&& vec);

		// copy assignment 
		//vector<T, Alloc> operator= (const vector<T, Alloc>& other); 
		vector& operator = (const vector& other);
		vector& operator = (vector&& other);

		//destructor
		~vector();

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
		//这里直接使用start还是使用begin()??
		bool empty() const{return begin()==end();}
		size_type size() const{return size_type(end()-begin());}
		size_type max_size() const{return size_type(-1)/sizeof(T);}
		size_type capacity() const { return size_type(end_of_storage-start); }

		void reserve(size_type n);
		void shrink_to_fit();
		
		//element access
		T& operator[] (size_type index);
		T at(size_type index);
		T& front();
		T& back();
		size_type data(const T&);

		//elements modifiers
		void assign(size_type poos);
		void assign(size_type pos, const T& value);

		void push_back(const T& value);
		T pop_back();

		void insert(size_t pos, const T& value);
		void erase(size_t pos);

		void clear();

		void resize();

		void reverse();

		void swap(size_t pos1,size_t pos2);
	
	// for inner use
	private:

};
#endif
