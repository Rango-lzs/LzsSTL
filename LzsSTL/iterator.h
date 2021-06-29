#ifndef ITERATOR_HH
#define ITERATOR_HH

#include "iterator_traits.h"
#include <cstddef>
struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag: public input_iterator_tag {};
struct bidirectional_iteartor_tag : public forward_iterator_tag {};
struct random_iterator_tag: public bidirectional_iteartor_tag{};

// what is Distance 
template<class T, class Distance>
class input_iterator
{
    typedef input_iterator_tag iterator_category;
    typedef T  value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef Distance difference_type;
};


template<class Category, class T, class Distance=ptrdiff_t,
    class Pointer=T*, class Reference= T&>
struct iterator
{
    
    typedef Category    iterator_category;
    typedef T           value_type;
    typedef Distance    difference_type;
    typedef Pointer     pointer;
    typedef Reference   reference;
};


template<typename iterator>
struct iterator_traits {

	typedef typename    iterator::iterator_category         iterator_category;
	typedef typename    iterator::value_type                value_type;
	typedef typename    iterator::pointer_type              pointer_type;
	typedef typename    iterator::const_value_type          const_value_type;
	typedef typename    iterator::const_pointer_type        const_pointer_type;
	typedef typename    iterator::reference_type            reference_type;
	typedef typename    iterator::const_reference_type      const_reference_type;
	typedef typename    iterator::pointer_different_type    pointer_different_type;
};


//partial specification for raw pointer and const raw pointer
template <class T>
struct iterator_traits<T*> {
	// typedef typename    T           value_type;
	// their should not have typename , typename only declare for nested type define
	typedef    random_iterator_tag  itrator_category;
	typedef    T           value_type;
	typedef    T*          pointer_type;
	typedef    const T     const_value_type;
	typedef    const T*    const_pointer_type;
	typedef    T&          reference_type;
	typedef    const T&    const_reference_type;
	typedef    ptrdiff_t   pointer_different_type;
};

template <class T>
struct iterator_traits<const T*> {

	typedef    random_iterator_tag  itrator_category;
	typedef     T           value_type;
	typedef     T*          pointer_type;
	typedef     const T     const_value_type;
	typedef     const T*    const_pointer_type;
	typedef     T&          reference_type;
	typedef     const T&    const_reference_type;
	typedef     ptrdiff_t   pointer_different_type;
};

template<class Iterator>
inline typename iterator_traits<Iterator>::itrerator_category
iteratory_category(const Iterator &){

    typedef typename iterator_traits<Iterator>::iterator_category category;
    return category();
}

//这个函数决定某个迭代器的 value type
template <class Iterator>
inline typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&) {
	return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}


// implemet advance by function overload throuth the category
template<class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type 
distance(InputIterator first, InputIterator last)
{
    typedef typename iterator_traits<InputIterator>::iterator_category category;
    __distance(InputIterator first,InputIterator last, category());
}

#endif
