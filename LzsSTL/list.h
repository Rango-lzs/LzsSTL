#ifndef _LZS_LIST_HH
#define _LZS_LIST_HH

#include "iterator.h"
#include "type_traits.h"
#include "alloc.h"
#include "uninitailized.h"
#include "allocator.h"
#include <list>
namespace Lzs {

	template<class T>
	struct list_node {
		typedef list_node<T>*  node_pointer;
		node_pointer next;
		node_pointer prev;
		T data;
		list_node() { data = 0; prev = next = nullptr; }
		list_node(T value = 0, node_pointer p = nullptr, node_pointer n = nullptr)
			:data(value), prev(p), next(n) {}
	};

	template<class T, class Ref, class Ptr>
	struct list_iterator : public iterator<bidirectional_iteartor_tag, T> {
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
		list_iterator(link_type x) : node(x) {} // 隐式转换
		list_iterator(const iterator& x) : node(x.node) {}

		//操作符重载 ==  ++  ++()  !=
		bool operator == (const iterator& other)
		{
			return node == other.node;
		}

		bool operator != (const iterator& other)
		{
			return !(*this == other);
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

	template<class T, class Alloc = alloc<list_node<T>>>
	class list {
	public:
		typedef T						value_type;
		typedef value_type*				pointer;
		typedef const value_type*		const_pointer;
		typedef value_type&				reference;
		typedef const value_type&		const_reference;
		typedef size_t					size_type;
		typedef ptrdiff_t				difference_type;

		typedef Alloc									allocate_type;
		typedef list_iterator<T, T&, T*>				iterator;
		typedef list_iterator<T, const T&, const T*>	const_iterator;
		//typedef reverse_iterator<iterator>				reverse_iterator;
		//typedef reverse_iterator<const_iterator>		const_reverse_iterator;

		typedef list_node<T>*		link_type;

		typedef allocator<list_node<T>, Alloc> data_allocate;

	private:
		list_node<T>* node;

	public:
		list() { __initialize(); }

		explicit list(size_type n) :list(n, T()) {}  //委托构造函数

		explicit list(size_type n, const T& value);

		template<class InputIterator>
		list(InputIterator first, InputIterator last);

		list(const list& x);

		list(list&& x);

		list& operator= (const list&x)
		{
			node = x.node();
		}

		list& operator=(list&& x)
		{
			node = x.node();
			x.node = nullptr;
		}

		~list() {};
	public:
		//迭代器相关操作
		iterator begin() { return (link_type)node->next; }
		const_iterator begin() const { return (link_type)node->next; }
		iterator end() { return node; }
		const_iterator end() const { return node; }
		//reverse_iterator rbegin() { return reverse_iterator(end()); }
		//const_reverse_iterator rbegin() { return reverse_iterator(end()); }
		//reverse_iterator rend() { return reverse_iterator(begin()); }
		//const_reverse_iterator rend() { return reverse_iterator(begin()); }
		//容量相关操作
		bool empty() const { return node->next = node; }
		size_type size() const { return (size_type)distance(begin(), end()); }
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
		void insert(iterator positon, size_type n, const T& x);
		template<class InputIterator>
		void insert(iterator pos, InputIterator first, InputIterator last);

		iterator erase(iterator pos);
		iterator erase(iterator fitst, iterator last);
		void clear();

		void push_front(const T& x) { insert(begin(), x); }
		void push_back(const T& x) { insert(end(), x); }
		void pop_front() { erase(begin()); }
		void pop_back() { iterator tmp = end(); erase(--tmp); } //erase(--end()) ???

		void resize(size_type new_size, const T& x);
		void resize(size_type new_size) {  }
		//void swap(list& x) { ::swap(node, x.node);}

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
		void __insert_dispatch(iterator pos, Integer n, Integer value, __true_type);

		template<class InputIterator>
		void  __insert_dispatch(iterator pos, InputIterator first, InputIterator last, __false_type);

		void __transfer(iterator pos, iterator first, iterator last);
	};

	

	template <class T, class Alloc>
	list<T, Alloc>::list(size_type n, const T& value):list()
	{
		assign(n, value);
	}

	template <class T, class Alloc>
	template<class InputIterator>
	list<T, Alloc>::list(InputIterator first, InputIterator last)
	{
		assign(first, last);
	}

	template <class T, class Alloc>
	list<T, Alloc>::list(const list<T, Alloc>& x)
	{
		//swap(x);
	}

	template <class T, class Alloc>
	list<T, Alloc>::list(list<T, Alloc>&& x)
	{
		//swap(x);
		//x.node = nullptr;
	}

	template <class T, class Alloc>
	template<class InputIterator>
	void list<T, Alloc>::assign(InputIterator first, InputIterator last)
	{
		typedef typename __is_integer<InputIterator>::is_integer integer;
		__assign_dispatch(first, last, integer());
	}

	template<class T, class Alloc>
	typename list<T, Alloc>::iterator
		list<T, Alloc>::insert(iterator pos, const T& x)
	{
		/*link_type pnode = __create_node(x);
		link_type tmp = pos->next;
		pos->next = pnode;
		pnode->next = tmp;
		tmp->prev = pnode;
		pnode->prev = pos;
		return tmp;*/

		link_type pnode = __create_node(x);
		pnode->next = pos.node;
		pnode->prev = pos.node->prev;
		pos.node->prev->next = pnode;
		pos.node->prev = pnode;
		return pnode;
	}

	template<class T, class Alloc>
	void
		list<T, Alloc>::insert(iterator pos, size_type n, const T& x)
	{
		while (--n)
		{
			insert(pos, x);
			--pos;
		}
	}

	template<class T, class Alloc>
	template<class InputIterator>
	void list<T, Alloc>::insert(iterator pos, InputIterator first, InputIterator last)
	{
		typedef typename is_integer<first>::value_type integer;
		__insert_dispatch(pos, first, last, integer());
	}

	template<class T, class Alloc>
	typename list<T, Alloc>::iterator
		list<T, Alloc>::erase(iterator pos)
	{
		iterator tmp = pos++;
		pos.node->prev->next = pos.node->next;
		pos.node->next->prev = pos.node->prev;
		__destroy_node(pos.node); // lzs 
		return tmp;
	}

	template <class T, class Alloc>
	typename list<T, Alloc>::iterator
		list<T, Alloc>::erase(iterator first, iterator last)
	{
		while (first != last)
		{
			iterator  cur = first;
			first++;
			erase(cur);  // 必须先临时保存  不然erase后就失效了
		}
		return last;
	}

	template<class T, class Alloc>
	void list<T, Alloc>::resize(size_type new_size, const T& x)
	{
		iterator i = begin();
		size_type len = 0;
		while (i != end() && len < new_size)
		{
			++i;
			++len;
		}
		if (len == new_size)
			erase(i, end());
		else
			insert(end(), new_size - len, x);
	}

	template<class T, class Alloc>
	void list<T, Alloc>::clear()
	{
		link_type first = (link_type)node->next;
		link_type last = (link_type)node;
		while (first != last)
		{
			link_type cur = first;
			first = first->next;
			__destroy_node(cur);
		}

		node->prev = node->next = node;
	}

	template<class T, class Alloc>
	void list<T, Alloc>::splice(iterator pos, list& x)
	{
		if (!x.empty())
		{
			__transfer(pos, x.begin(), e.end());
		}
	}

	template<class T, class Alloc>
	void list<T, Alloc>::splice(iterator pos, list&, iterator i)
	{
		iterator j = i;
		++j;
		if (pos == i || pos == j) return;
		__transfer(pos, i, j);
	}

	template<class T, class Alloc>
	void list<T, Alloc>::splice(iterator pos, list& x, iterator first, iterator last)
	{
		if (first != last)
		{
			__transfer(pos, first, last);
		}
	}

	template<class T, class Alloc>
	void list<T, Alloc>::remove(const T& value)
	{
		iterator first = begin();
		iterator last = end();
		while (first != last)
		{
			iterator cur = first;
			++cur;
			if (*first == value) erase(first);
			first = cur;
		}
	}

	template<class T, class Alloc >
	template<class Predicate>
	void list<T, Alloc>::remove_if(Predicate pred)
	{	
		//Predicate 是一个函数对象  重载opearator()
		iterator first = begin();
		iterator last = end();
		while (first != last)
		{
			iterator cur = first;
			++cur;
			if (Predicate(*first)) erase(first);
			first = cur;
		}
	}

	template<class T, class Alloc>
	void list<T, Alloc>::unique()
	{
		iterator first = begin();
		iterator last = end();
		if (first == last) return;
		while (first != last)
		{
			iterator cur = first;
			++cur;
			if (*first==*cur) 
				erase(cur);
			else
				first = cur;
		}
	}

	//按什么样的规则合并？
	template<class T, class Alloc>
	void list<T, Alloc>::merge(list<T, Alloc>& x)
	{
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = x.begin();
		iterator last2 = x.end();
		while (first1 != last1 && first2！ = last2)
		{
			if (*first2 < *first1) {
				iterator next = first2;
				__transfer(first1, first2, ++next);
				first2 = next;
			}
			else
				++first1;
		}
		if (first2 != last2)
			__transfer(last1, first2, last2);
	}

	template<class T, class Alloc>
	void list<T, Alloc>::sort()
	{
		if (node->next == node || (link_type)node->next->next == node) return;
		list<T, Alloc>::carry;
		list<T, Alloc>::counter[64];
		int fill = 0;
		while (!empty()) {
			carry.splice(carry.begin(), *this, begin());
			int i = 0;
			while (i < fill && !counter[i].empty()) {
				counter[i].merge(carry);
				carry.swap(counter[i++])
			}
			carry.swap(counter[i]);
			if (i == fill) ++fill;
		}
		for (int i = 1; i < fill; ++i) {
			counter[i].merge(counter[i - 1]);
		}
		swap(counter[fill - 1]);
	}

	template<class T, class Alloc>
	void list<T, Alloc>::reverse() {
		if (node->next == node || (link_type)node->next->next == node) return;
		iterator first = begin();
		++first;
		while (first != end()) {
			iterator old = first;
			++first;
			__transfer(begin(), old, first);
		}
	}

	template<class T, class Alloc>
	typename list<T, Alloc>::link_type
		list<T, Alloc>::__create_node(const T& x = T()) {
		link_type p = data_allocate::allocate();
		try {
			data_allocate::construct(p, list_node<T>(x));
		}
		catch (...) {
			data_allocate::deallocate(p);
		}
		return p;
	}

	template<class T, class Alloc>
	void list<T, Alloc>::__destroy_node(link_type p) {
		::destroy(&p->data);
		data_allocate::deallocate(p);
	}

	template<class T, class Alloc>
	void list<T, Alloc>::__initialize()
	{
		//初始化list  构造一个node 自己指向自己
		node = __create_node();
		node->prev = node;
		node->next = node;
	}

	template<class T, class Alloc>
	void list<T, Alloc>::__fill_assign(size_type n, const T& value) {
		iterator i = begin();
		for (; i != end() && n > 0; ++i, --n)
			*i = value;
		if (n > 0)
			insert(end(), n, value);
		else
			erase(i, end());
	}

	template<class T, class Alloc>
	template <class Integer>
	void list<T,Alloc>::__assign_dispatch(Integer n, Integer value, __true_type)
	{
		__fill_assign((size_type)n, (T)value);
	}

	template<class T, class Alloc>
	template<class InputIterator>
	void list<T, Alloc>::__assign_dispatch(InputIterator first2, InputIterator last2, __false_type)
	{
		iterator first1 = begin();
		iterator last1 = end();
		for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
			*first1 = *first2;
		}
		if (first2 = last2)
			erase(first1, last1);
		else
			insert(last1, first2, last2);
	}

	template<class T, class Alloc>
	void list<T, Alloc>::__fill_insert(iterator pos, size_type n, const T& value)
	{
		for (;n>0;--n)
		{
			insert(pos, value);
		}
	}

	template<class T,class Alloc>
	template<class Integer>
	void list<T, Alloc>::__insert_dispatch(iterator pos, Integer n, Integer value, __true_type)
	{
		__fill_insert(pos, (size_type)n, (T)value);
	}

	template<class T, class Alloc>
	template<class InputIterator>
	void list<T, Alloc>::__insert_dispatch(iterator pos, InputIterator first, InputIterator last, __false_type)
	{
		for (;first!=last;++first)
		{
			insert(pos, *first);
		}
	}

	template<class T, class Alloc>
	void list<T, Alloc>::__transfer(iterator pos, iterator first, iterator last)
	{
		if (pos != last)
		{
			last.node->prev->next = pos.node;
			first.node->prev->next = last.node;
			pos.node->prev->next = first.node;
			link_type tmp = (link_type)pos.node->prev;
			pos.node->prev = first.node->prev;
			first.node->prev = tmp;
		}
	}

	template<class T, class Alloc>
	inline bool operator == (const list<T, Alloc>& x, const list<T, Alloc>& y)
	{
		typedef typename list<T, Alloc>::const_iterator const_iterator;
		const_iterator first1 = x.begin();
		const_iterator first2 = y.begin();
		const_iterator last1 = x.end();
		const_iterator last2 = y.end();
		while (first1!=last1 && first2!=last2 && *first1=*first2)
		{
			++first1;
			+first2;
		}
		return first1 == last1 && first2 == last2;
	}


} //end of Lzs

#endif

/*
          
*/