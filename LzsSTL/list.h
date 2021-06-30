#ifndef _LZS_LIST_HH
#define _LZS_LIST_HH


namespace Lzs {

template<class T>
struct list_node {
	list_node* next;
	list_node* prev;
	T* data;
};

template<class T, class Alloc = alloc<T>>
class list {

};
template<class T>
class list_iterator {

};

} //end of Lzs

#endif