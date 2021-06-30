#ifndef _LZS_LIST_HH
#define _LZS_LIST_HH

#include "iterator.h"
#include <list>
namespace Lzs {

template<class T>
struct list_node {
	typedef list_node<T>*  node_pointer;
	node_pointer next;
	node_pointer prev;
	T data;
	list_node() { data = 0; prev = next = nullptr; }
	list_node(T value=0, node_pointer p=nullptr,node_pointer n=nullptr)
		:data(value),prev(p),next(n) {}
};

template<class T, class Ref, class Ptr>
class list_iterator: public iterator<bidirectional_iteartor_tag,T> {
	//这可以算是偏特化吗
	typedef list_iterator<T, T&, T*>				iterator;
	typedef list_iterator<T, const T&, const T*>	const_iterator;
	typedef list_iterator<T, Ref, Ptr>				self;

	typedef T		value_type;
	typedef Ptr		pointer;
	typedef Ref		reference;
	typedef size_t	size_type;

	typedef list_node<T>* link_type;

	link_type node;  // iterator 和 node 关联起来

	list_iterator() {}
	list_iterator(link_type x): node(x) {} // 隐式转换
	list_iterator(const iterator& x): node(x.node){}

	//操作符重载 ==  ++  ++()  !=
	bool operator == (const iterator& other)
	{
		return node == other.node;
	}

	bool operator != (const iterator& other)
	{
		return !(*this==other)
	}

	reference operator *()
	{
		return (*node).data;
	}

	pointer operator ->()
	{
		return &(operator*());
	}

	self operator++()
	{
		iterator tmp = *this;
		node = node->next;
		return tmp;
	}

	self operator++(int)
	{
		node = node->next;
		return *this;
	}

	self operator--()
	{
		iterator tmp = *this;
		node = node->prev;
		return tmp;
	}

	self operator--(int)
	{
		node = node->prev;
		return *this;
	}

}; 

template<class T, class Alloc = alloc<T>>
class list {
public:
	typedef T						value_type;
	typedef value_type*				pointer;
	typedef const value_type*		const_pointer;
	typedef value_type&				reference;
	typedef cosnt value_type&		const_reference;
	typedef size_t					size_type;
	typedef ptrdiff_t				difference_type;

	typedef Alloc									allocate_type;
	typedef list_iterator<T, T&, T*>				iterator;
	typedef list_iterator<T, const T&, const T*>	const_iterator;
	typedef reverse_iterator<iterator>				reverse_iterator;
	typedef reverse_iterator<const_pointer>			const_reverse_iterator;

	typedef list_node<T>*		link_type;

	typedef allocator<list_node<T>, Alloc> data_allocate;

private:
	list_node* node;

public:
	list() { __initialize(); }
	explicit list(size_type n);
	explicit list(size_type n, const T& value);
	template<class InputIterator>
	list(InputIterator first, InputIterator last);

	list(const list& x);

	list(list&& x);

	list& operator= (const list&x);

	list& operator=(list&& x);

	~list();
public:
	//迭代器相关操作
	iterator begin() { return (link_type)node->next; }
	const_iterator begin() const { return (link_type)node->next;}
	iterator end() { return node; }
	const_iterator end() const { return node; }
	reverse_iterator rbegin() { return reverse_iterator(end()); }
	const_reverse_iterator rbegin() { return reverse_iterator(end()); }
	reverse_iterator rend() { return reverse_iterator(begin()); }
	const_reverse_iterator rend() { return reverse_iterator(begin()); }
	//容量相关操作
	bool empty() const { return node->next = node; }
	size_type size() const { return (size_type)distance(begin(), end())); }
	size_type max_size() const { return size_type(-1); }

	//访问元素操作
	reference front() { return *begin(); }
	const_reference front() const { return *begin(); }
	reference back() { return *(--end()); } // *(end()--) 可以吗
	const_reference back() const { return *(--end()); }

	//调整容器相关操作
	void assign(size_type n, const T& value) { __fill_assign(n, value); }
	void assign(size_type n) { __fill_assign(n, T()); }
	template<class InputIterator>
	void assign(InputIterator first, InputIterator last);

	iterator insert(iterator position, const T& x);
	iterator insert(iterator position) { insert(position, T()); }
	void insert(iterator positon, size_type n, cosnt T& x);
	template<class InputIterator>
	void insert(iterator pos, InputIterator first, InputIterator last);

	iterator erase(iterator pos);
	iterator erase(iterator fitst, iterator last);
	void clear();

	void push_front(const T& x) { insert(begin(), x); }
	void push_back(const T& x) { insert(end(), x)); }
	void pop_front() { erase(begin()); }
	void pop_back() { iterator tmp = end(); erase(--tmp); } //erase(--end()) ???

	void resize(size_type new_size, const T& x);
	void resize(size_type new_size) { resize(new_size, T()); }
	void swap(list& x) { ::swap(node, x.node);}

	//容器相关操作
	void splice(iterator pos, list& x);
	void splice(iterator pos, list& x, iterator i);
	void splice(iterator pos, list& x, iterator first, iterator last);

	void remove(const T& value);
	template<class Predicate>
	void remove_if(Predicate pred);

	void unique();
	void merge(list& x);
	void sort();
	void reverse();

	allocate_type get_allocate() { return allocate_type(); }

private:
	//内部成员函数
	link_type __create_node(const T& x = T());
	void __destroy_node(link_type p);
	void __initialize();
	void __fill_assign(size_type n, const T& value);

	template <class Integer>
	void __assign_dispatch(Integer n, Integer value, __true_type);

	template<class InputIterator>
	void  __assign_dispatch(InputIterator first, InputIterator last, __false_type);

	void __fill_insert(iterator pos, size_type n, const T& value);

	template <class Integer>
	void __insert_dispatch(Integer n, Integer value, __true_type);

	template<class InputIterator>
	void  __insert_dispatch(InputIterator first, InputIterator last, __false_type);

	void __transfer(iterator pos, iterator first, iterator last);

};



} //end of Lzs

#endif

/*
          
*/